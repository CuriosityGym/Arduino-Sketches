// sets up and initialize CGShield
#include <Adafruit_NeoPixel.h>
#include <CGShield.h>
#include <Wire.h>         // Require for I2C communication
CGShield fs;             // Instanciate CGShield instance

#include <SoftwareSerial.h>
#include "U8glib.h"
SoftwareSerial BTserial(10, 9); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
 
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 
char receivedData = ' ';
int melody[]= {2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};

void setup() 
    {
      Serial.begin(9600);
      Serial.println("Arduino is ready");
 
      // HC-05 default serial speed for commincation mode is 9600
      BTserial.begin(115200);  
    }
 
void loop()
    {
 
      // Keep reading from HC-05 and send to Arduino Serial Monitor
      if (BTserial.available())
         {  
          receivedData = BTserial.read();
          Serial.write(receivedData);
          u8g.firstPage();  
          do 
           { 
             u8g.setFont(u8g_font_timB10);
             u8g.setPrintPos(0, 10); 
             u8g.print("Token Number" );
             u8g.setFont(u8g_font_fub25);
             u8g.setPrintPos(0, 50); 
             u8g.print(receivedData);
           } while( u8g.nextPage() );
          for(byte thisNote = 0; thisNote < 20; thisNote++) 
             {
               color(0,thisNote*5,0); 
               // to calculate the note duration, take one second divided by the note type.
               //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
               int noteDuration = 1000/noteDurations[thisNote];
               tone(A1, melody[thisNote],noteDuration);
               // to distinguish the notes, set a minimum time between them.
               // the note's duration + 30% seems to work well:
               int pauseBetweenNotes = noteDuration * 1.5;
               delay(pauseBetweenNotes);
           
               // stop the tone playing:
               noTone(8);
               color(0,0,0);
               delay(250);
             }
      }
    /*// Keep reading from Arduino Serial Monitor and send to HC-05
    if (Serial.available())
    {
        receivedData =  Serial.read();
        BTserial.write(receivedData);  
    }*/
}
