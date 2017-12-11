/*
Cayenne Ethernet Example

This sketch connects to the Cayenne server using an Arduino Ethernet Shield W5100
and runs the main communication loop.

The Cayenne Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Set the token variable to match the Arduino token from the Dashboard.
2. Compile and upload this sketch.

For Cayenne Dashboard widgets using digital or analog pins this sketch will automatically
send data on those pins to the Cayenne server. If the widgets use Virtual Pins, data
should be sent to those pins using virtualWrites. Examples for sending and receiving
Virtual Pin data are under the Basics folder.
*/

//#define CAYENNE_DEBUG         // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneEthernet.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "bla bla";

#include <DHT.h>
#define DHTPIN 14 // what pin we're connected to 14 (A0)
// note GPIO5 is D1 at NodeMCU - http://www.14core.com/wp-content/uploads/2015/06/Node-MCU-Pin-Out-Diagram1.png
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE,11);
float t, h;

// Virtual Pin of the TMP36 widget.
#define VIRTUAL_PIN V1

// Analog pin the TMP36 is connected to.
const int tmpPin = 0;

// Voltage to the TMP36. For 3v3 Arduinos use 3.3.
const float voltage = 5.0; 

#include <CayenneTemperature.h>
TMP36 tmpSensor(tmpPin, voltage);


void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Arduino Uno & W5100 - Cayenne IoT");
  dht.begin();
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

}

// This function is called when the Cayenne widget requests data for the Virtual Pin.
CAYENNE_OUT(VIRTUAL_PIN)
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  // This command writes the temperature in Celsius to the Virtual Pin.
  Cayenne.celsiusWrite(VIRTUAL_PIN, tmpSensor.getCelsius());
  Cayenne.celsiusWrite(1, t);
  Cayenne.virtualWrite(2, h);
 
}
