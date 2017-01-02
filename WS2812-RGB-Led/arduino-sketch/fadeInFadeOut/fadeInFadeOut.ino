// sets up and initialize CGShield
#include <Adafruit_NeoPixel.h>
#include <CGShield.h>
#include <Wire.h>         // Require for I2C communication
CGShield fs;             // Instanciate CGShield instance
#define Led 1
#define WS2812_PIN 6
float delayval = 0.0001;

byte RED[] =  { 255,   0,   0};
byte GREEN[] ={   0, 255,   0};
byte BLUE[]= {   0,   0, 255};


void setup()
  {
    initialize();
  }


void loop()
    {
      while(1)
           { 
             FadeInFadeOut();
           }
    }

void FadeInFadeOut()
   {
     for(int j = 0; j < 3; j++ ) 
       { 

       // Fade IN

       for(int k = 0; k < 256; k++) { 

       switch(j)  
       { 

        case 0: color(k,0,0); break;

        case 1: color(0,k,0); break;

        case 2: color(0,0,k); break;

      }
      update();
      delay(3);

    }

    // Fade OUT

    for(int k = 255; k >= 0; k--) { 

      switch(j) { 

        case 0: color(k,0,0); break;

        case 1: color(0,k,0); break;

        case 2: color(0,0,k); break;

      }
       update();
      delay(3);

    }

  }

}
