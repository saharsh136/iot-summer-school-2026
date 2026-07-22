const int gasPin = A0;
const int flamePin = 7;

const int greenLED = 10;
const int yellowLED = 11;
const int redLED = 12;

const int buzzer = 9;

unsigned long previousMillis = 0;
bool buzzerState = false;

void setup() {

  Serial.begin(9600);

  pinMode(flamePin, INPUT_PULLUP);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(buzzer, OUTPUT);

  Serial.println("MQ-2 Warming Up...");
  delay(5000);
  Serial.println("System Ready");
}

void loop() {

  int gasValue = analogRead(gasPin);

  int gasPercent = map(gasValue, 0, 1023, 0, 100);

  bool flameDetected = (digitalRead(flamePin) == LOW);

  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);

  noTone(buzzer);

  String status = "";

  if (!flameDetected && gasPercent < 30) {

    status = "SAFE";

    digitalWrite(greenLED, HIGH);

  }

  else if (!flameDetected && gasPercent >= 30 && gasPercent <= 60) {

    status = "WARNING";

    digitalWrite(yellowLED, HIGH);

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 500) {

      previousMillis = currentMillis;

      buzzerState = !buzzerState;

      if (buzzerState)
        tone(buzzer, 1000);
      else
        noTone(buzzer);
    }

  }

  else if (!flameDetected && gasPercent > 60) {

    status = "DANGER (GAS)";

    digitalWrite(redLED, HIGH);

    tone(buzzer, 1000);

  }

  else if (flameDetected && gasPercent <= 60) {

    status = "DANGER (FLAME)";

    digitalWrite(redLED, HIGH);

    tone(buzzer, 1800);

  }

  else {

    status = "DANGER (GAS + FLAME)";

    digitalWrite(redLED, HIGH);

    tone(buzzer, 2200);

  }

  Serial.print("GAS: ");
  Serial.print(gasPercent);
  Serial.print("% | ");

  Serial.print("FLAME: ");

  if (flameDetected)
    Serial.print("DETECTED");
  else
    Serial.print("NONE");

  Serial.print(" | STATUS: ");
  Serial.println(status);

  delay(1000);
}
