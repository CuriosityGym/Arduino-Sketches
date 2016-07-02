/*  


    In this example we are using NRF24L01 radio and Arduino to
    communicate with other Arduino and NRF24L01 using OLED display.
    Here we are creating a local mesh between two radios(NRF24L01)
    using <RF24.h> and <RF24Network.h library>. For OLED we are using
    <Adafruit_GFX.h> and <Adafruit_SSD1306.h> libraries.
    For every new message buzzer will alert user and message will be 
    diplayed on OLED.

*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


// Hardware configuration
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(7,8);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;

boolean send_message = false;
char messageToSend[32] = "";
int melody[]= {2000,2000,2000,2000};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 8,8,8,8 };

void setup(void)
{
  Serial.begin(57600);
  Serial.println("NRF24L01 MESSENGER");
  display.clearDisplay();
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextSize(0.5);
  display.setTextColor(WHITE);         
  display.setCursor(10,15);
  display.println("NRF24L01   MESSENGER");
  display.display();
  delay(2000);
  display.clearDisplay();
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop() 
    {
       // Pump the network regularly
       network.update();
       if (send_message)   // if there is typed message on serial monitor ready to send
        {
          RF24NetworkHeader header1(/*to node*/ other_node);
          boolean message = network.write(header1, messageToSend, 32);   // send message to other user
          if (message)
            {
              Serial.print("Sender: ");   // print message on serial monitor
              Serial.println(messageToSend);
              send_message = false;
            }
          
          else
            
              Serial.println("could not write....\n"); // if it is failed to send message prompt error 
        }  
    
     //// Is there anything ready for us?
     while (network.available() )
       { // If so, grab it and print it out
         RF24NetworkHeader header;
         char messageToReceive[32] = "";
         boolean recieve = false;
         while (!recieve)
            {
              recieve = network.read(header, messageToReceive ,32);
              Serial.print("Reciver: ");   // print recived data on serial monitor
              Serial.println(messageToReceive);
              
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
             // print received message on OLED    
             display.clearDisplay();        
             display.setCursor(0,15);
             display.println(messageToReceive);
             display.display();
             delay(2000);
            }
         
        }
   
     if (Serial.available())  // type message on serial monitor
       { 
         int data = Serial.readBytesUntil('\n', messageToSend, 32);  //read typed message from serial monitor
         messageToSend[data] = '\0';
         send_message = true;
       }
   }
