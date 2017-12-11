/*
Cayenne Ethernet example modified by Nicu FLORICA (niq_ro), 11.12.2017, Craiova-Romania
*/

//#define CAYENNE_DEBUG         // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneEthernet.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "blabla";

#include <DHT.h>
#define DHTPIN 14 // what pin we're connected to 14 (A0)
// note GPIO5 is D1 at NodeMCU - http://www.14core.com/wp-content/uploads/2015/06/Node-MCU-Pin-Out-Diagram1.png
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE,11);
float t, h;
unsigned long lastMillis = 0;

#define VIRTUAL_PIN 3
#define RELAY_DIGITAL_PIN 2

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Arduino Uno & W5100 - Cayenne IoT");
  dht.begin();
 // set digital pin to output
  pinMode(RELAY_DIGITAL_PIN, OUTPUT);
  
  Cayenne.begin(token);
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celcius Humidity: "); 
  Serial.println(h);
}

void loop()
{
  Cayenne.run();
    if (millis() - lastMillis > 10000) {
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celsius Humidity: "); 
  Serial.println(h);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Cayenne.celsiusWrite(1, t);
  Cayenne.virtualWrite(2, h);
  lastMillis = millis();
    }

}


CAYENNE_IN(VIRTUAL_PIN)
{
  // get value sent from dashboard
  int currentValue = getValue.asInt(); // 0 to 1

  // assuming you wire your relay as normally open
  if (currentValue == 0) {
    digitalWrite(RELAY_DIGITAL_PIN, HIGH);
  } else {
    digitalWrite(RELAY_DIGITAL_PIN, LOW);
  }
}
