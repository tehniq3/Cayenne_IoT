/*
This example shows how to connect to Cayenne using an Ethernet W5100 shield and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Set the Cayenne authentication info to match the authentication info from the Dashboard.
2. Compile and upload the sketch.
3. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

//#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "blabla1";
char password[] = "blabla2";
char clientID[] = "blabla3";

#include <DHT.h>
#define DHTPIN 14 // what pin we're connected to 14 (A0)
// note GPIO5 is D1 at NodeMCU - http://www.14core.com/wp-content/uploads/2015/06/Node-MCU-Pin-Out-Diagram1.png
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE,11);
float t, h;

#define VIRTUAL_CHANNEL 3
#define ACTUATOR_PIN 2

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Arduino Uno & W5100 - Cayenne IoT");
  dht.begin();
 // set digital pin to output
  pinMode(ACTUATOR_PIN, OUTPUT);
  Cayenne.begin(username, password, clientID);
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celcius Humidity: "); 
  Serial.println(h);
}

void loop() {
  Cayenne.loop();
}

// This function is called when the Cayenne widget requests data for the Virtual Pin.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  Cayenne.celsiusWrite(1, t);
  Cayenne.virtualWrite(2, h);
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celcius Humidity: "); 
  Serial.println(h);
}

// This function is called when data is sent from Cayenne.
CAYENNE_IN(VIRTUAL_CHANNEL)
{
  int value = getValue.asInt();
  CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL, ACTUATOR_PIN, value);
  // Write the value received to the digital pin.
  digitalWrite(ACTUATOR_PIN, value);
}
