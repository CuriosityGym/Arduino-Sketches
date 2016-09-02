//#include <Adafruit_NeoPixel.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "U8glib.h"

// Hardware configuration
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(7,8);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 1;

// Address of the other node
const uint16_t other_node = 0;

boolean send_message = false;
char messageToSend[32] = "";  //

int melody[]= {2000,2000,2000,2000};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 8,8,8,8 };

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int motor_left[] = {2, 3};
int motor_right[] = {4, 5};
#define trigPin 9
#define echoPin 6
long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;           // interval at which to blink (milliseconds)
int duration, distance;
int x, y;
//#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

// --------------------------------------------------------------------------- Setup
void setup() 
     { 
       Serial.begin(57600);
       pinMode(trigPin, OUTPUT);
       pinMode(echoPin, INPUT);
       // Setup motors
       for(byte i = 0; i < 2; i++)
          {
            pinMode(motor_left[i], OUTPUT);
            pinMode(motor_right[i], OUTPUT);
          }
       u8g.setFont(u8g_font_unifont);
       
    //    strip.begin();
  //strip.show(); // Initialize all pixels to 'off'
       SPI.begin();
       radio.begin();
       network.begin(/*channel*/ 90, /*node address*/ this_node);
       u8g.firstPage();  
       do 
        {
          u8g.setPrintPos(0, 10); 
          u8g.print("Robot " );
          u8g.setPrintPos(45, 10); 
          u8g.print(this_node);
        } while( u8g.nextPage() );   
     

      }


// --------------------------------------------------------------------------- Loop
void loop() 
    {  
      // Pump the network regularly
       network.update();
       
       unsigned long currentMillis = millis();
       
       if(currentMillis - previousMillis > interval)
         {
           // save the last time you blinked the LED 
           previousMillis = currentMillis;   
           digitalWrite(trigPin, HIGH);
           digitalWrite(trigPin, LOW);
           duration = pulseIn(echoPin, HIGH);
           distance = (duration/2) / 29.1;
            x = (x + distance)%100;
           y = (x + y + distance)%100;
           Serial.println(distance);
           
         }  
       if (distance < 20)
          { //strip.setPixelColor(0, 255,0,0);    //turn every third pixel o      }
            //strip.show();
            Serial.println("right");
            motor_stop();
            delay(250);
            turn_right();
            delay(600);
            motor_stop();
            delay(250);
            
            inputString = String(x) + "   " + String(y);
            RF24NetworkHeader header1(/*to node*/ other_node);
            boolean message = network.write(header1, inputString.c_str(), 32); // send message to other user
            if (message)
               {
                 Serial.print("Robot2: ");  // print message on serial monitor
                 Serial.println(inputString);
                 send_message = false;
               }
          
            else
               {
                 Serial.println("could not write....\n");  // if it is failed to send message prompt error
              
                // stringComplete=false;
                // inputString="";
               }
          }    
    
       else
          {
            Serial.println("forward");
            drive_forward();
             // Is there anything ready for us?
            while (network.available() )   
                  { 
                    // If so, grab it and print it out
                    RF24NetworkHeader header;
                    char messageToRecieve[32] = "";
                    boolean recieve = false;
                    while (!recieve)
                          {
                            recieve = network.read(header, messageToRecieve , 32 );
                            Serial.print("Robot2: ");   // print recived data on okserial monitor
                            Serial.println(messageToRecieve);
                            for(int thisNote = 0; thisNote < 4; thisNote++) 
                               {
                                // to calculate the note duration, take one second divided by the note type.
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
                             u8g.firstPage();  
                             do 
                               {
                                 u8g.drawStr(0,10, "Robot1 says: "); 
                               //  u8g.drawStr(0,25, "found obstacle at: ");
                               //  u8g.setPrintPos(0, 44); 
                               //  u8g.print(messageToRecieve); 
                               } while( u8g.nextPage() );   
                         }
                 }
          }
    }     


// --------------------------------------------------------------------------- Drive

void motor_stop()
     {
       digitalWrite(motor_left[0], LOW);
       digitalWrite(motor_left[1], LOW);

       digitalWrite(motor_right[0], LOW);
       digitalWrite(motor_right[1], LOW);
       delay(25);
     }


void drive_forward()
     {
       digitalWrite(motor_left[0], HIGH);
       digitalWrite(motor_left[1], LOW);

       digitalWrite(motor_right[0], HIGH);
       digitalWrite(motor_right[1], LOW);
     }


void turn_right()
     {
       digitalWrite(motor_left[0], HIGH);
       digitalWrite(motor_left[1], LOW);

       digitalWrite(motor_right[0], LOW);
       digitalWrite(motor_right[1], HIGH);
     }
 /*    
