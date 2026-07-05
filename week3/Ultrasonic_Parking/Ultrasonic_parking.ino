const int trigPin = 9;
const int echoPin = 10;

const int greenLED = 2;
const int yellowLED = 3;
const int redLED = 4;

const int buzzer = 8;

unsigned long previousMillis = 0;
bool outputState = false;

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration * 0.034) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm   ");

  unsigned long currentMillis = millis();

  // ---------- SAFE ----------
  if (distance > 50) {

    Serial.println("SAFE");

    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);

    noTone(buzzer);
  }

  else if (distance > 20) {

    Serial.println("WARNING");

    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);

    if (currentMillis - previousMillis >= 500) {

      previousMillis = currentMillis;

      outputState = !outputState;

      if (outputState)
        tone(buzzer, 1000);
      else
        noTone(buzzer);
    }
  }

  // ---------- DANGER ----------
  else if (distance > 10) {

    Serial.println("DANGER");

    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

    if (currentMillis - previousMillis >= 200) {

      previousMillis = currentMillis;

      outputState = !outputState;

      if (outputState)
        tone(buzzer, 1500);
      else
        noTone(buzzer);
    }
  }

  
  else {

    Serial.println("STOP");

    if (currentMillis - previousMillis >= 100) {

      previousMillis = currentMillis;

      outputState = !outputState;

      digitalWrite(greenLED, outputState);
      digitalWrite(yellowLED, outputState);
      digitalWrite(redLED, outputState);
    }

    tone(buzzer, 2000);
  }
}
