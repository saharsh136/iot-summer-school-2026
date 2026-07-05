const int ldrPin = A0;
const int pirPin = 2;
const int ledPin = 9;

const int ldrThreshold = 500;

unsigned long motionTime = 0;
bool motionDetected = false;

void setup() {

  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int ldrValue = analogRead(ldrPin);
  bool motion = digitalRead(pirPin);

  // ---------- DAY ----------
  if (ldrValue > ldrThreshold) {

    analogWrite(ledPin, 0);

    printEvent("Daylight detected - LED OFF");

    motionDetected = false;

    delay(1000);
    return;
  }

  // ---------- NIGHT ----------
  if (motion) {

    analogWrite(ledPin, 255);

    motionTime = millis();

    if (!motionDetected) {
      printEvent("Motion detected - LED Full Brightness");
      motionDetected = true;
    }
  }

  else {

    if (motionDetected) {

      if (millis() - motionTime >= 30000) {

        analogWrite(ledPin, 51);

        printEvent("No motion - LED Dimmed (20%)");

        motionDetected = false;
      }

      else {

        analogWrite(ledPin, 255);
      }
    }

    else {

      analogWrite(ledPin, 51);
    }
  }

  delay(100);
}

void printEvent(String event) {

  unsigned long seconds = millis() / 1000;

  int hh = seconds / 3600;
  int mm = (seconds % 3600) / 60;
  int ss = seconds % 60;

  if (hh < 10) Serial.print("0");
  Serial.print(hh);
  Serial.print(":");

  if (mm < 10) Serial.print("0");
  Serial.print(mm);
  Serial.print(":");

  if (ss < 10) Serial.print("0");
  Serial.print(ss);

  Serial.print(" EVENT: ");
  Serial.println(event);
}
