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
#include <Adafruit_NeoPixel.h>
#include <idIoTwareShield.h>
#include <Wire.h>         // Require for I2C communication
idIoTwareShield fs;             // Instanciate CGShield instance

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "8ae41dd66020477a85c1b23d63e22203";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CuriosityGym-BCK";
char pass[] = "#3Twinkle3#";

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
      // Set console baud rate
      debugSerial.begin(9600);
      delay(10);
      // Set ESP8266 baud rate
      EspSerial.begin(ESP8266_BAUD);
      delay(10);

      Blynk.begin(auth, wifi, ssid, pass);
    }

//http://blynk-cloud.com/yourAPIkey/update/V0?value=250?pin=V0&value=0&pin=V0&value=0

//create virtual pin 1 and read value from that pin
BLYNK_WRITE(V0)
           {
             int red = param[0].asInt();
             int green = param[1].asInt();
             int blue = param[2].asInt();
             color(red,green,blue); //Set color received from Blynk API 
           }

void loop()
{
  Blynk.run();
}

