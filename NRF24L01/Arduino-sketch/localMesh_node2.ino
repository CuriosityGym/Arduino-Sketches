/*  

    In this example we are using NRF24L01 radio and Arduino to
    communicate with other Arduino and NRF24L01 using Serial Monitor.
    Here we are creating a local mesh between two radios(NRF24L01)
    using <RF24.h> and <RF24Network.h library>.  


*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 1;

// Address of the other node
const uint16_t other_node = 0;

boolean send_message = false;
char messageToSend[32] = "";  //


void setup(void)
{
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
              Serial.print("Sender: ");  // print message on serial monitor
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
              Serial.print("Reciver: ");   // print recived data on serial monitor
              Serial.println(messageToRecieve);
            }
 
        }
    
     if (Serial.available()) // type message on serial monitor
       { 
         int data = Serial.readBytesUntil('\n', messageToSend, 32);//read typed message from serial monitor
         messageToSend[data] = '\0';
         send_message = true;  
       }
   }
