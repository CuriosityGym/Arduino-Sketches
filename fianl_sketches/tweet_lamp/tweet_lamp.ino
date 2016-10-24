/*
    idIoTWare Shield Twitter Lamp
    Attention: Please install all libraries from our Github Repository to enable this example to run.
    
    In this example we are using ESP8266-01 Wifi Module  and onboard WS2812 multicolor led.
    
    In this example we are using Blynk HTTP RESTful API. 
    Blynk HTTP RESTful API allows to easily read and write values to/from Pins in Blynk apps and 
    Hardware (microcontrollers and microcomputers like Arduino, Raspberry Pi, ESP8266, Particle, etc.).
    Every PUT request will update Pin's state both in apps and on the hardware. Every GET request will 
    return current state/value on the given Pin. We also provide simplified API so you can do updates via GET requests.
    You can read on how to do it here //http://docs.blynkapi.apiary.io/#reference/0/get-pin-value/write-pin-value-via-get?console=1
    Use your Auth Token number in url to get data from server.
    
    In this example we have used ESP8266_Lib.h and BlynkSimpleShieldEsp8266.h  library to make communication between arduino
    and Blynk HTTP RESTful API. We have used Heroku Deployment Application to receive tweet from our twitter handle and then this application 
    sends get request to Blynk HTTP RESTful API.   
*/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>


#include <Adafruit_NeoPixel.h>

#define PIN 6
int tempPin = A0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxxxx";  //replace it with your token number

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "abcd";    
char pass[] = "***********";

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
      debugSerial.begin(115200);
      delay(10);
      // Set ESP8266 baud rate
      EspSerial.begin(ESP8266_BAUD);
      delay(10);

      Blynk.begin(auth, wifi, ssid, pass);
      strip.begin();
      strip.show();
    }
//http://blynk-cloud.com/8ae41dd66020477a85c1b23d63e22203/update/V0?value=250?pin=V0&value=0&pin=V0&value=0

//create virtual pin 1 and read value from that pin
BLYNK_WRITE(V0)
           {
             int red = param[0].asInt();
             int green = param[1].asInt();
             int blue = param[2].asInt();
             strip.setPixelColor(0,red,green,blue); //Set color received from twitter handle 
             strip.show();
           }


             

void loop()
{
  Blynk.run();
}


