const int LED = LED_BUILTIN;

int blinkCounter = 0;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(9600);
  Serial.println("Serial Command Interface Ready");
}

void loop() {

  if (Serial.available()) {

    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "LED_ON") {

      digitalWrite(LED, HIGH);
      Serial.println("LED is ON");
    }

    else if (command == "LED_OFF") {

      digitalWrite(LED, LOW);
      Serial.println("LED is OFF");
    }

    else if (command.startsWith("BLINK_")) {

      String number = command.substring(6);

      if (number.length() == 1 &&
          number[0] >= '1' &&
          number[0] <= '9') {

        int times = number.toInt();

        for (int i = 0; i < times; i++) {

          digitalWrite(LED, HIGH);
          delay(300);

          digitalWrite(LED, LOW);
          delay(300);
        }

        blinkCounter += times;

        Serial.print("Blinked ");
        Serial.print(times);
        Serial.println(" times");
      }

      else {

        Serial.println("ERROR: Invalid blink count");
      }
    }

    else if (command == "STATUS") {

      Serial.print("LED State: ");

      if (digitalRead(LED))
        Serial.println("ON");
      else
        Serial.println("OFF");

      Serial.print("Blink Counter: ");
      Serial.println(blinkCounter);
    }

    else if (command == "RESET") {

      blinkCounter = 0;
      Serial.println("Blink counter reset");
    }

    else {

      Serial.println("ERROR: Unknown command");
    }
  }
}
