// ============================================================
//  ESP32 Home Automation + Fire/Gas Safety - Local Dashboard Server
// ============================================================
//  - ESP32 boards POST JSON data to /api/update/home and /api/update/gas
//  - Server holds the latest reading from each board in memory
//  - Every update is instantly pushed to all connected browsers
//    over WebSocket (no polling -> lowest possible latency)
//  - Dashboard (public/index.html) renders the live data
// ============================================================

const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const path = require('path');
const os = require('os');

// ---- Find all candidate LAN IPs, labeled by adapter name ----
// (Laptops often have virtual adapters from VirtualBox/VMware/Hyper-V/Docker
//  that also show up here - those won't be reachable by the ESP32 over WiFi)
function getLocalIps() {
  const interfaces = os.networkInterfaces();
  const candidates = [];
  for (const name of Object.keys(interfaces)) {
    for (const iface of interfaces[name]) {
      if (iface.family === 'IPv4' && !iface.internal) {
        candidates.push({ name, address: iface.address });
      }
    }
  }
  return candidates;
}

// Best-effort guess at the "real" adapter, deprioritizing common virtual ones
function guessBestIp(candidates) {
  const virtualPatterns = /virtualbox|vmware|hyper-v|vethernet|docker|loopback/i;
  const preferred = candidates.filter(c => !virtualPatterns.test(c.name));
  return (preferred[0] || candidates[0] || { address: 'localhost' }).address;
}

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server, path: '/ws' });

const PORT = process.env.PORT || 3000;

// ---- In-memory latest state (this is all we need, no DB required) ----
let state = {
  home: {
    temp: null,
    hum: null,
    motion: false,
    fan: false,
    online: false,
    lastSeen: null,
    enabled: true   // manual override: false = paused/disconnected by the dashboard
  },
  gas: {
    gasValue: null,
    gasLeak: false,
    flame: false,
    status: 'UNKNOWN',
    online: false,
    lastSeen: null,
    enabled: true   // manual override: false = paused/disconnected by the dashboard
  }
};

// Consider a device "offline" if we haven't heard from it in this long
const OFFLINE_TIMEOUT_MS = 5000;

app.use(express.json());

// Serve the dashboard
app.use(express.static(path.join(__dirname, 'public')));

// ---- Helper: broadcast current state to every connected browser ----
function broadcast(type) {
  const payload = JSON.stringify({ type, state });
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(payload);
    }
  });
}

// ---- ESP32 #1: Smart Home board (DHT11 + PIR + fan + buzzer) ----
app.post('/api/update/home', (req, res) => {
  const { temp, hum, motion, fan } = req.body;

  state.home = {
    temp: typeof temp === 'number' ? temp : state.home.temp,
    hum: typeof hum === 'number' ? hum : state.home.hum,
    motion: !!motion,
    fan: !!fan,
    online: true,
    lastSeen: Date.now(),
    enabled: state.home.enabled // preserve whatever the dashboard has set
  };

  broadcast('home');
  // Tell the board whether it's allowed to run its actuators right now
  res.json({ ok: true, enabled: state.home.enabled });
});

// ---- ESP32 #2: Gas/Flame safety board (MQ2 + flame sensor + relays) ----
app.post('/api/update/gas', (req, res) => {
  const { gasValue, gasLeak, flame, status } = req.body;

  state.gas = {
    gasValue: typeof gasValue === 'number' ? gasValue : state.gas.gasValue,
    gasLeak: !!gasLeak,
    flame: !!flame,
    status: status || state.gas.status,
    online: true,
    lastSeen: Date.now(),
    enabled: state.gas.enabled // preserve whatever the dashboard has set
  };

  broadcast('gas');
  // Tell the board whether it's allowed to run its actuators right now
  res.json({ ok: true, enabled: state.gas.enabled });
});

// ---- Manual override from the dashboard: pause/resume a board's actuators ----
// This does NOT sever the ESP32's WiFi connection (not possible over plain HTTP) -
// it tells the board to stop driving its fan/relay and report itself as paused.
// The board keeps checking in, so it can be re-enabled remotely at any time.
app.post('/api/control/:device', (req, res) => {
  const { device } = req.params;
  const { enabled } = req.body;

  if (device !== 'home' && device !== 'gas') {
    return res.status(404).json({ ok: false, error: 'Unknown device' });
  }
  if (typeof enabled !== 'boolean') {
    return res.status(400).json({ ok: false, error: 'enabled must be true or false' });
  }

  state[device].enabled = enabled;
  broadcast(device);
  res.json({ ok: true, device, enabled });
});

// ---- Initial state fetch for the dashboard on page load ----
app.get('/api/state', (req, res) => {
  res.json(state);
});

// ---- WebSocket: send current state as soon as a browser connects ----
wss.on('connection', (ws) => {
  ws.send(JSON.stringify({ type: 'init', state }));
});

// ---- Mark a board offline if it stops sending data ----
setInterval(() => {
  const now = Date.now();
  let changed = false;

  if (state.home.online && state.home.lastSeen && now - state.home.lastSeen > OFFLINE_TIMEOUT_MS) {
    state.home.online = false;
    changed = true;
    broadcast('home');
  }
  if (state.gas.online && state.gas.lastSeen && now - state.gas.lastSeen > OFFLINE_TIMEOUT_MS) {
    state.gas.online = false;
    changed = true;
    broadcast('gas');
  }
}, 1000);

server.listen(PORT, '0.0.0.0', () => {
  const candidates = getLocalIps();
  const bestGuess = guessBestIp(candidates);

  console.log(`\n✅ Dashboard server running`);
  console.log(`   Local:   http://localhost:${PORT}\n`);
  console.log(`   Available network adapters:`);
  candidates.forEach(c => {
    const marker = c.address === bestGuess ? '  <-- likely your WiFi/LAN (try this first)' : '';
    console.log(`     [${c.name}]  http://${c.address}:${PORT}${marker}`);
  });
  console.log(`\n   If the marked one doesn't work, run "ipconfig" (Windows) or "ifconfig"`);
  console.log(`   (Mac/Linux) and use the IPv4 address under your actual WiFi adapter\n`);
});
