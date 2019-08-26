/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example shows how to fetch data using WebHook GET method
  App project setup:
    WebHook widget on V0, method: GET, url: /pin/
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include "U8glib.h"

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "80a9953b71584ee4854e0fbf71823901";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "The Promised LAN";
char pass[] = "imagineyourquestion";
// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(10, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);
int interval = 10000;
bool updateThingspeak = false;
bool sendMail = false;
bool soilDry = true; 
bool motorStarted = false;
bool plantWatered = false;
unsigned long startTimer = 0;
int motorPin = 9;
int moistureSensor = 2;

BlynkTimer timer;


void sendMailToUser()
{
  // You can perform HTTPS requests even if your hardware alone can't handle SSL
  // Blynk  can also fetch much bigger messages,
  // if hardware has enough RAM (set BLYNK_MAX_READBYTES to 4096)
  //Blynk.virtualWrite(V0, "https://api.sunrise-sunset.org/json?lat=50.4495484&lng=30.5253873&date=2016-10-01");

   Blynk.virtualWrite(V0, "https://maker.ifttt.com/trigger/soilMoisture/with/key/c1BVwMuv-fI8ryLpLIihJe");
}


void logToThingspeak(){
  Blynk.virtualWrite(V0, "https://api.thingspeak.com/api_key=*************&field1=1");  
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  // Set console baud rate
  debugSerial.begin(115200);
  delay(10);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  //timer.setInterval(16000L, getBitcoinValue);
   digitalWrite(motorPin, LOW);  // motor off
 
  }

void loop()
{
  Blynk.run();
  //timer.run();
  unsigned long currentMillis = millis();
  //digitalWrite(motorPin, LOW);  // motor off
      
  // read the input pins:
  int moistureSensorState = digitalRead(moistureSensor);  
  if(moistureSensorState == HIGH && soilDry == false)   // if soil is dry then water the plant
    { 
      soilDry = true; 
      motorStarted = true;
      startTimer = millis();    
    }
  if(motorStarted == true && soilDry == true)   // if soil is dry then water the plant
    { 
      digitalWrite(motorPin, HIGH); 
      if((currentMillis - startTimer > 10000) && moistureSensorState == LOW)
        {
          digitalWrite(motorPin, LOW);
          sendMail = true;
          updateThingspeak = true; 
          motorStarted = false;
          soilDry = false;
          plantWatered = true;
        }
      if(currentMillis - startTimer > interval && moistureSensorState == HIGH)
        {
         digitalWrite(motorPin, LOW);   
        }
      if(((currentMillis - startTimer) > (interval + 40000)) && moistureSensorState == HIGH)
        {
         digitalWrite(motorPin, HIGH);   
        }
      if(((currentMillis - startTimer) > (interval + 50000)) && moistureSensorState == HIGH)
        {
         digitalWrite(motorPin, HIGH);   
        }     
    }
  if(plantWatered == true){
    sendMailToUser();
    logToThingspeak();             
    Serial.print("Plant Watered");  
    motorStarted = false;
    soilDry = false;
    sendMail = false;                                        
    updateThingspeak = false;
  }
      
}
