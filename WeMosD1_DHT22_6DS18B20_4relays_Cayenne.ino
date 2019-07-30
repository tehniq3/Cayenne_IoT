// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 
// small changes by Nicu FLORICA aka niq_ro, 27.11.2017, Craiova - Romania
// see http://nicuflorica.blogspot.ro/search?q=nodemcu
// see http://nicuflorica.blogspot.ro/search?q=wemos
// see http://nicuflorica.blogspot.ro/search?q=cayenne
// this sketch control 4 relays, read 1 sensor DHT22(AM2302) sensor and 6 sensors DS18B29

#define desepin 2 // GPIO2 = D9
#define dehastepin 5 // GPIO5 = D3
#define releu1 4 // GPIO4 = D4
#define releu2 14 // GPIO14 = D5
#define releu3 12 // GPIO12 = D6
#define releu4 13 // GPIO13 = D7

// https://lastminuteengineers.com/multiple-ds18b20-arduino-tutorial/
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS desepin  

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

int deviceCount = 0;
float tempC;
float te[6];

#include <DHT.h>
#define DHTPIN dehastepin

//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE,11);

float t, h;
#define ledexterior 0 // external led is as D3

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "wifi_network_name";
char wifiPassword[] = "pasword_network";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "char username";
char password[] = "char password";
char clientID[] = "char clientID";

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("GLASS HOUSE remote - Cayenne IoT");
  dht.begin();
  //delay(1000);
  
  sensors.begin();  // Start up the library
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  
  pinMode(releu1, OUTPUT);
  digitalWrite(releu1, LOW);   // Turn the LED off 
  pinMode(releu2, OUTPUT);
  digitalWrite(releu2, LOW);   // Turn the LED off 
  pinMode(releu3, OUTPUT);
  digitalWrite(releu3, LOW);   // Turn the LED off 
  pinMode(releu4, OUTPUT);
  digitalWrite(releu4, LOW);   // Turn the LED off   
  
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celcius Humidity: "); 
  Serial.println(h);
  
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
  Cayenne.loop();

  //Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
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

   // Send command to all the sensors for temperature conversion
  sensors.requestTemperatures(); 
  
  // Display temperature from each sensor
//  for (int i = 0;  i < deviceCount;  i++)
   for (int i = 0;  i < 6;  i++)
  {
    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(" : ");
    tempC = sensors.getTempCByIndex(i);
    te[i] = tempC;
    Serial.print(tempC);
    Serial.print((char)176);//shows degrees character
    Serial.print("C  |  ");
    Serial.print(te[i]);
    Serial.print((char)176);//shows degrees character
    Serial.println("C  |  ");
 /*   
    Serial.print(DallasTemperature::toFahrenheit(tempC));
    Serial.print((char)176);//shows degrees character
    Serial.println("F");
*/
  }
 

    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    //Cayenne.virtualWrite(0, lastMillis);
    //Some examples of other functions you can use to send data.
    Cayenne.virtualWrite(0, h);
    Cayenne.celsiusWrite(1, t);

    Cayenne.celsiusWrite(6, te[0]);
    Cayenne.celsiusWrite(7, te[1]);
    Cayenne.celsiusWrite(8, te[2]);
    Cayenne.celsiusWrite(9, te[3]);
    Cayenne.celsiusWrite(10, te[4]);
    Cayenne.celsiusWrite(11, te[5]);
  
    //Cayenne.luxWrite(2, 700);
    //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
 
lastMillis = millis();
  }



}  // end main loop

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

CAYENNE_IN(2)  // button 1
  {   
    int currentValue=getValue.asInt();
     if(currentValue==1)
     {
      digitalWrite(releu1,HIGH);
      }
      else
      {
      digitalWrite(releu1,LOW); 
      }
  }

CAYENNE_IN(3)  // button 2
  {   
    int currentValue=getValue.asInt();
     if(currentValue==1)
     {
      digitalWrite(releu2,HIGH);
      }
      else
      {
      digitalWrite(releu2,LOW); 
      }
  }

CAYENNE_IN(4)  // button 3
  {   
    int currentValue=getValue.asInt();
     if(currentValue==1)
     {
      digitalWrite(releu3,HIGH);
      }
      else
      {
      digitalWrite(releu3,LOW); 
      }
  }

CAYENNE_IN(5)  // button 4
  {   
    int currentValue=getValue.asInt();
     if(currentValue==1)
     {
      digitalWrite(releu4,HIGH);
      }
      else
      {
      digitalWrite(releu4,LOW); 
      }
  }
