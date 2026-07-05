#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

#define LDR_PIN 34
#define TRIG_PIN 5
#define ECHO_PIN 18

DHT dht(DHTPIN, DHTTYPE);

long getDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0)
    return -1;

  return duration * 0.0343 / 2;
}

void setup()
{
  Serial.begin(115200);

  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  analogReadResolution(10);
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int rawLDR = analogRead(LDR_PIN);

  int lightPercent = map(rawLDR, 0, 1023, 0, 100);

  lightPercent = constrain(lightPercent, 0, 100);

  String lightState;

  if (lightPercent >= 70)
    lightState = "Bright";
  else if (lightPercent >= 40)
    lightState = "Normal";
  else
    lightState = "Dark";

  long distance = getDistance();

  Serial.println("=== SENSOR LOG ===");

  Serial.print("Time      : ");
  Serial.print(millis());
  Serial.println(" ms");

  Serial.print("Temp      : ");

  if (isnan(temperature))
    Serial.print("Error");
  else
  {
    Serial.print(temperature, 1);
    Serial.print(" C");
  }

  Serial.print(" | Humidity: ");

  if (isnan(humidity))
    Serial.print("Error");
  else
  {
    Serial.print(humidity, 0);
    Serial.print("%");
  }

  Serial.println();

  Serial.print("Light     : ");
  Serial.print(lightPercent);
  Serial.print("% (");
  Serial.print(lightState);
  Serial.println(")");

  Serial.print("Distance  : ");

  if (distance == -1)
    Serial.println("Out of Range");
  else
  {
    Serial.print(distance);
    Serial.println(" cm");
  }

  Serial.println("==================");
  Serial.println();

  delay(5000);
}
