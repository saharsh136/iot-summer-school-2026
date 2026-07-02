const int LED_PIN = 4;
const int BLINK_DELAY = 1000; // Delay in milliseconds

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, HIGH); // Turn LED ON
    delay(BLINK_DELAY);

    digitalWrite(LED_PIN, LOW);  // Turn LED OFF
    delay(BLINK_DELAY);
}
