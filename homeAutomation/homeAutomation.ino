/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * Control a color gradient on NeoPixel strip using a slider!
 *
 * For this example you need NeoPixel library:
 *   https://github.com/adafruit/Adafruit_NeoPixel
 *
 * App project setup:
 *   Slider widget (0...500) on V1
 *
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include "pitches.h"
#include <Adafruit_NeoPixel.h>

#define PIN 6
int tempPin = A0;
const byte touchpadPin= 4;
int counter;
const byte pushbutton= 5;
int counterr;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "8ae41dd66020477a85c1b23d63e22203";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CuriosityGym-BCK";
char pass[] = "#3Twinkle3#";

int led = 2;

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(10, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

void setup()
    { 
      pinMode(led, OUTPUT);
      // Set console baud rate
      debugSerial.begin(9600);
      delay(10);
      // Set ESP8266 baud rate
      EspSerial.begin(ESP8266_BAUD);
      delay(10);

      Blynk.begin(auth, wifi, ssid, pass);
     
    }


         
BLYNK_WRITE(V1) // Widget in the app READs Virtual Pin V3 with the certain frequency
          {
            // This command writes Arduino's uptime in seconds to Virtual Pin V2
           int value = param.asInt();
      
           if(value == 1)
             {
               digitalWrite(led, HIGH);               
             }
           else
             {
               digitalWrite(led, LOW);
             }  
         }  
  
       

void loop()
{
  Blynk.run();
}

