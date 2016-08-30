/*  

    In this example we are using NRF24L01 radio and Arduino to
    communicate with other Arduino and NRF24L01 using Serial Monitor.
    Here we are creating a local mesh between two radios(NRF24L01)
    using <RF24.h> and <RF24Network.h library>.  


*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "U8glib.h"
//
// Hardware configuration
//

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

void setup(void)
{ u8g.setFont(u8g_font_unifont);
  Serial.begin(57600);
  Serial.println("NRF24L01 MESSENGER");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop() 
    {
       // Pump the network regularly
       network.update();
       
       if (send_message)  // if there is typed message on serial monitor ready to send
        {
          RF24NetworkHeader header1(/*to node*/ other_node);
          boolean message = network.write(header1, messageToSend, 32); // send message to other user
          if (message)
            {
              Serial.print("Robot2: ");  // print message on serial monitor
              Serial.println(messageToSend);
              send_message = false;
            }
          
          else
            
              Serial.println("could not write....\n");  // if it is failed to send message prompt error
        }  
    
    
     // Is there anything ready for us?
     while (network.available() )   
       { // If so, grab it and print it out
         RF24NetworkHeader header;
         char messageToRecieve[32] = "";
         boolean recieve = false;
         while (!recieve)
            {
              recieve = network.read(header, messageToRecieve , 32 );
              Serial.print("Robot1: ");   // print recived data on okserial monitor
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
                 u8g.setPrintPos(0, 25); 
                 u8g.print(messageToRecieve); 
               } while( u8g.nextPage() );   
            }
 
        }
    
     if (Serial.available()) // type message on serial monitor
       { 
         byte data = Serial.readBytesUntil('\n', messageToSend, 32);//read typed message from serial monitor
          messageToSend[data-1] = 0x20;
         send_message = true;  
       }
   }
