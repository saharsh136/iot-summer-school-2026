#define PIR_PIN 13
#define LED_PIN 2
#define BUZZER_PIN 14

bool previousMotion = false;
bool alertMode = false;

unsigned long lastMotionTime = 0;

unsigned long eventLog[5];
int eventIndex = 0;

void setup() {

  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  Serial.println("=================================");
  Serial.println("Motion Activated Security System");
  Serial.println("System in STANDBY");
  Serial.println("=================================");
}

void loop() {

  bool motion = digitalRead(PIR_PIN);

  if (motion && !previousMotion) {

    alertMode = true;

    lastMotionTime = millis();

    digitalWrite(LED_PIN, HIGH);

    tone(BUZZER_PIN, 1000);
    delay(200);
    noTone(BUZZER_PIN);

    eventLog[eventIndex] = millis();
    eventIndex = (eventIndex + 1) % 5;

    Serial.println();
    Serial.println("******** ALERT ********");
    Serial.print("Motion Detected at ");
    Serial.print(millis());
    Serial.println(" ms");

    Serial.println("Last Motion Events:");

    for (int i = 0; i < 5; i++) {
      if (eventLog[i] != 0) {
        Serial.print(i + 1);
        Serial.print(". ");
        Serial.print(eventLog[i]);
        Serial.println(" ms");
      }
    }

    Serial.println("***********************");
  }

  if (motion) {
    lastMotionTime = millis();
  }

  if (alertMode && (millis() - lastMotionTime >= 60000)) {

    alertMode = false;

    digitalWrite(LED_PIN, LOW);

    Serial.println();
    Serial.println("No Motion for 60 seconds");
    Serial.println("Returning to STANDBY");
  }

  previousMotion = motion;

  delay(100);
}
