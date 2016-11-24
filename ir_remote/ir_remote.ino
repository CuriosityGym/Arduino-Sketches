
#include <Adafruit_NeoPixel.h>
#include <IRremote.h>
#include "U8glib.h"

int RECV_PIN = 3;

IRrecv irrecv(RECV_PIN);

decode_results results;

#define PIN 6
float tempInC;
int wait= 1;
#define time 10
int freq;
int counter;
bool test1=true;
bool test2=false;
bool test3=false;
bool test4=false;
bool test5=false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);	// Fast I2C / TWI 

void setup()
    {
      irrecv.enableIRIn(); // Start the receiver
      strip.begin();
      strip.show(); // Initialize all pixels to 'off' 
      u8g.setFont(u8g_font_timB14);
    }

void loop() 
     { 
       
       if (irrecv.decode(&results) && test1 == true)
          { 
            delay(500);
            counter+=1;
            u8g.firstPage();  
            do 
              {
                u8g.drawStr(20,20,"Multicolor");
                u8g.drawStr(15,45,"Led Effects");
              } while( u8g.nextPage() );
            
            irrecv.resume(); // Receive the next value
            test1=false; test2=true;
            colorWipe(strip.Color(255, 0, 0), 1500); // Red
            colorWipe(strip.Color(0, 255, 0), 1500); // Green
            colorWipe(strip.Color(0, 0, 255), 1500); // Blue
           // FadeInFadeOut();
            rainbow(20);
           fadeInFadeOut(); 
            colorWipe(strip.Color(0, 0, 0), 250); // Blue
          }
       delay(100);
   
       if (irrecv.decode(&results) && test2 == true) 
          { 
            delay(500);
            counter+=1;
            u8g.firstPage();  
            do 
              {
                u8g.drawStr(0,20,"Happy Birthday");
                u8g.drawStr(30,45,"Melody");
              } while( u8g.nextPage() );
            irrecv.resume(); // Receive the next value
            test3=false;test3=true;
            happy_birthday();
          }
       delay(100);
      
        if (irrecv.decode(&results) && test3 == true)
          { 
            delay(500);
            counter+=1;
            irrecv.resume(); // Receive the next value
            test4=false;test4=true;
            u8g.firstPage();  
            do 
              {
                u8g.drawStr(0,20,"Police Siren");
              } while( u8g.nextPage() );
            
            police_Siren();
            colorWipe(strip.Color(0, 0, 0), 250); // Blue
            
          }
       if (irrecv.decode(&results) && test4 == true)
          { 
            delay(500);  
            counter+=1;
            irrecv.resume(); // Receive the next value}
           }
       if(counter == 4 && test4 == true)
              {           
                int val1 = analogRead(A0);
                int val = analogRead(A0);
                float voltage = val * (5000.0 / 1024.0);    // voltage at pin milliVolts
                tempInC = (voltage / 10);  // voltage to C
                temperature();
                delay(500);
          //    }
        if (irrecv.decode(&results))
           { 
             counter=0;
             test4=false;test1=true;
           }
        delay(100);
    }
  }

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
     {
       for(uint16_t i=0; i<strip.numPixels(); i++) 
          {
            strip.setPixelColor(i, c);
            strip.show();
            delay(wait);
          }
     }

void rainbow(uint8_t wait) 
     {
       uint16_t i, j;
       for(j=0; j<256; j++) 
          {
            for(i=0; i<strip.numPixels(); i++) 
               {
                 strip.setPixelColor(i, Wheel((i+j) & 255));
               }
            strip.show();
            delay(wait);
          }
     }

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
     {
       if(WheelPos < 85) 
         {
           return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
         } 
       else if(WheelPos < 170) 
         {
           WheelPos -= 85;
           return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
         } 
       else 
         {
           WheelPos -= 170;
           return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
         }
     }

void fadeInFadeOut() 
     { 
    for(int j = 0; j < 3; j++ ) 
        { 
         // Fade IN
         for(int k = 0; k < 256; k++)
            { 
              switch(j)  
                    { 
                      case 0: colorWipe(strip.Color(k,0,0),1); break;
                      case 1: colorWipe(strip.Color(0,k,0),1); break;
                      case 2: colorWipe(strip.Color(0,0,k),1); break;
                    }
              delay(10);
            }

         // Fade OUT
         for(int k = 255; k >= 0; k--) 
            { 
              switch(j) 
                    { 
                      case 0: colorWipe(strip.Color(k,0,0),1); break;
                      case 1: colorWipe(strip.Color(0,k,0),1); break;
                      case 2: colorWipe(strip.Color(0,0,k),1); break;
                    }
              delay(10);
            } 
        }
      }
   

 void happy_birthday()
     { 
       // notes in the melody:
       int melody[]= { 196, 196, 220, 196, 512, 247,  
                       196,196, 220,196, 294, 512, 
                       196, 196, 392,330,512, 247, 220, 
                       349, 349, 330, 512, 294, 512 };
       // note durations: 4 = quarter note, 8 = eighth note, etc.:
       int noteDurations[] = { 8,8,4,4,4,2,8,8,4,4,4,2,8,8,4,4,4,4,3,8,8,4,4,4,2 };
       // iterate over the notes of the melody:
       for (int thisNote = 0; thisNote < 26; thisNote++) 
           {
            // to calculate the note duration, take one second 
            // divided by the note type.
            //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
            int noteDuration = 1000/noteDurations[thisNote];
            tone(A1, melody[thisNote],noteDuration);
            // to distinguish the notes, set a minimum time between them.
            // the note's duration + 30% seems to work well:
            int pauseBetweenNotes = noteDuration * 1.60;
            delay(pauseBetweenNotes);
            // stop the tone playing:
            noTone(8);
           }
     }  


float temperature()
    { 

      // return degreesC;
      u8g.firstPage();  
      do 
       {
        u8g.drawStr(5,20,"Temperature:");
        //u8g.setFont(u8g_font_timB18);
        u8g.setPrintPos(15, 42); 
        u8g.print(tempInC );
        u8g.setPrintPos(59, 42); 
        u8g.print(char(176));
        u8g.setPrintPos(67, 42); 
        u8g.print("C");
       } while( u8g.nextPage() );
        
  
     // rebuild the picture after some delay
    // delay(50);
   }
  
void police_Siren() 
    {
      for(freq = 150; freq < 1800; freq += 1)
         {
           colorWipe(strip.Color(0,0,freq),1);
           //color(0,0,freq);  
           tone(A1, freq, time);     // Beep pin, freq, time
           delay(wait);
         }
      for(freq = 1800; freq > 150; freq -= 1) 
         {
            colorWipe(strip.Color(freq,0,0),1);
           //color(freq,0,0);
           tone(A1, freq, time);     // Beep pin, freq, time
           delay(wait);
         }
    }   
   
   
