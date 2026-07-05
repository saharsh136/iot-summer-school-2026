const int RED = 13;
const int YELLOW = 12;
const int GREEN = 11;
const int BUTTON = 7;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(9600);
}

void showLight(int red, int yellow, int green, const char *state, int duration) {
  digitalWrite(RED, red);
  digitalWrite(YELLOW, yellow);
  digitalWrite(GREEN, green);

  Serial.print("Time: ");
  Serial.print(millis());
  Serial.print(" ms | State: ");
  Serial.println(state);

  delay(duration);
}

void loop() {

  // Pedestrian button pressed
  if (digitalRead(BUTTON) == LOW) {
    showLight(HIGH, LOW, LOW, "PEDESTRIAN CROSSING - RED", 8000);
    return;
  }

  // Red
  showLight(HIGH, LOW, LOW, "RED", 5000);

  if (digitalRead(BUTTON) == LOW) {
    showLight(HIGH, LOW, LOW, "PEDESTRIAN CROSSING - RED", 8000);
    return;
  }

  // Yellow
  showLight(LOW, HIGH, LOW, "YELLOW", 2000);

  if (digitalRead(BUTTON) == LOW) {
    showLight(HIGH, LOW, LOW, "PEDESTRIAN CROSSING - RED", 8000);
    return;
  }

  // Green
  showLight(LOW, LOW, HIGH, "GREEN", 4000);
}
