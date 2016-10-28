/*
  idIoTware Shield Fastest Finger First
  Attention: Please install all libraries from our Github Repository to enable this example to run.
  
  In this example we are using NRF24L01 radio module, Pushbutton switch, Buzzer and an I2C OLED display to show which player pressed pushbutton first.
  Here, 4 shields act as Transmitter and One shield act as a Receiver. Each time when any user(transmitter) presses button, then arduino
  sends the user number to receiver through NRF24L01 radio module and receiver receives the input and makes decision which player pressed
  the button first.You can change the number of players and particular time after which receiver will take decision.
  
  In this example we have used RF24Network.h and RF24.h library for NRF24L01 radio module and u8glib.h library for OLED display.
  
  This sketch is for transmitter. In this example if user press the pushbutton then arduino sends the user number to receiver through NRF24L01. 
  If message is sent successfully then buzzer makes beep sound.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(7,8);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 02;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format

int pushButton = 5;
int buzzerPin = A1;


void setup()
    {
      Serial.begin(57600);
      Serial.println("PLAYER 2");
 
      SPI.begin();
      radio.begin();
      network.begin(/*channel*/ 90, /*node address*/ this_node);
  
      pinMode(pushButton, INPUT);   // make the pushbutton's pin an input 
      pinMode(buzzerPin, OUTPUT);   // make the buzzer's pin an output
   }

void loop() 
    {
      network.update();                          // Check the network regularly

     int pushButtonState = digitalRead(pushButton); //read state of pushbutton   
     
     if(pushButtonState == HIGH)  // chack weather button is HIGH
       { 
        Serial.print("Sending...");
        RF24NetworkHeader header(/*to node*/ other_node);
        bool ok = network.write(header,"2",32);
        if(ok)
          {
            Serial.println("ok.");
            digitalWrite(buzzerPin,HIGH);  // make buzzer high for half second
            delay(500);
            digitalWrite(buzzerPin,LOW);
          }
        else
        Serial.println("failed.");
       }
   }

