// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 
// small changes by Nicu FLORICA aka niq_ro, 27.11.2017, Craiova - Romania
// see http://nicuflorica.blogspot.ro/search?q=nodemcu

#include <DHT.h>
#define DHTPIN 5 // what pin we're connected to
// note GPIO5 is D1 at NodeMCU - http://www.14core.com/wp-content/uploads/2015/06/Node-MCU-Pin-Out-Diagram1.png
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE,11);

#define ledalbastru 2 // blue led GPIO2 is D4
#define ledrosu 16  //  red led GPIO16 is D0
float t, h;
#define ledexterior 0 // external led is as D3

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "Wifi_network_name";
char wifiPassword[] = "Wifi_network_password";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT Username";
char password[] = "MQTT Password";
char clientID[] = "Client ID";

unsigned long lastMillis = 0;

void setup() {
	Serial.begin(9600);
  Serial.println();
  Serial.println("NodeMCU 0.9 - Cayenne IoT");
  dht.begin();
  //delay(1000);
//  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(ledrosu, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(ledalbastru, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
//  digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED off 
  digitalWrite(ledrosu, HIGH);   // Turn the LED off 
  digitalWrite(ledalbastru, HIGH);   // Turn the LED off 
  digitalWrite(ledrosu, LOW);   // Turn the LED on 
//  digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
  delay(300);
  //digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED off 
  digitalWrite(ledrosu, HIGH);   // Turn the LED off 
  digitalWrite(ledalbastru, LOW);   // Turn the LED on 
  delay(300);
  digitalWrite(ledalbastru, HIGH);   // Turn the LED off 

  pinMode(ledexterior, OUTPUT);
  digitalWrite(ledexterior, LOW);   // Turn the LED off 
  
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
    
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		//Cayenne.virtualWrite(0, lastMillis);
		//Some examples of other functions you can use to send data.
		Cayenne.celsiusWrite(1, t);
    Cayenne.virtualWrite(0, h);
		//Cayenne.luxWrite(2, 700);
		//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
 
   digitalWrite(ledrosu, LOW);   // Turn the LED off 
//  digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
  delay(50);
  //digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED off 
  digitalWrite(ledrosu, HIGH);   // Turn the LED off 
  digitalWrite(ledalbastru, LOW);   // Turn the LED off 
  delay(50);
  digitalWrite(ledalbastru, HIGH);   // Turn the LED off 
	}
/*
if (t > 25.00)  digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
else digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED off 
*/
if (t > 27.00)  
{
  digitalWrite(ledrosu, LOW);   // Turn the LED on (Note that LOW is the voltage level
  Serial.println("depasire temperatura !");
}
else digitalWrite(ledrosu, HIGH);   // Turn the LED off 

if (h > 60.00) 
{
digitalWrite(ledalbastru, LOW);   // Turn the LED on (Note that LOW is the voltage level
Serial.println("depasire umiditate !");
}
else digitalWrite(ledalbastru, HIGH);   // Turn the LED off 


}  // end main loop

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

CAYENNE_IN(3)  // button
  {   
    int currentValue=getValue.asInt();
     if(currentValue==1)
     {
      digitalWrite(ledexterior,HIGH);
      }
      else
      {
      digitalWrite(ledexterior,LOW); 
      }
  }
