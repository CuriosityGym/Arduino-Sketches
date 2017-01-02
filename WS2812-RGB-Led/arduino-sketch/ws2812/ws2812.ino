/*
  
  The CGShield uses a addresable RGB LED (WS2812).
  The CGShield library abstracts the details and provides 
  you the following function:
  
  color(red, green, blue); // where RGBcolor is an array of the three color components
                           // like for Red color, color(250,0,0)  
                           // for Blue color, color(0,250,0)
                           // for Blue color, color(0,0,250)  

*/

// sets up and initialize CGShield
#include <Adafruit_NeoPixel.h>
#include <CGShield.h>
#include <Wire.h>         // Require for I2C communication
CGShield fs;             // Instanciate CGShield instance

void setup()
   {
     initialize(); 
   }


void loop()
   {
     color(255,0,255);
     update(); //This updates the led at once   
   }
