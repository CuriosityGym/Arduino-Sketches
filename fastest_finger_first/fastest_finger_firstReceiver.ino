/*
  idIoTware Shield Fastest Finger First
  Attention: Please install all libraries from our Github Repository to enable this example to run.
  
  In this example we are using NRF24L01 radio module, Pushbutton switch, Buzzer and an I2C OLED display to show which player pressed pushbutton first.
  Here, 4 shields act as Transmitter and One shield act as a Receiver. Each time when any user(transmitter) presses button, then arduino
  sends the user number to receiver through NRF24L01 radio module and receiver receives the input and makes decision which player pressed
  the button first.You can change the number of players and particular time after which receiver will take decision.
  
  In this example we have used RF24Network.h and RF24.h library for NRF24L01 radio module and u8glib.h library for OLED display.
  This sketch is for receiver. In this example receiver receives input from users and decides which user pressed the pushbutton first and shows the 
  number of that user on OLED display and makes beep sound on buzzer 
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "U8glib.h"

RF24 radio(7,8);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);       // Network uses that radio
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 01;   // Address of the other node in Octal format

int buzzerPin = A1;   //buzzer is connected to analog pin 1 (A1) on idIoTware Shield

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 

int noOfPlayers = 4;
int takeDecision = 3000; // take decision which player pressed button first after 3 seconds.
unsigned long elapsedTime;      //varible to keep track of time
unsigned long recievedMessage1_Time; //vaiable to keep time of first received message
unsigned long recievedMessage2_Time; //vaiable to keep time of second received message
unsigned long recievedMessage3_Time; //vaiable to keep time of third received message
unsigned long recievedMessage4_Time; //vaiable to keep time of fourth received message
unsigned long lastReceivedMessage_Time;  // variable to keep track of time each time new message received

void setup()
    { 
      u8g.firstPage();  
      do 
       { 
         u8g.setFont(u8g_font_timB12);   //set font size
         u8g.drawStr(10,20,"Fastest Finger");
         u8g.drawStr(40,40," First");
       } while( u8g.nextPage() );
      delay(2000); 
                 
      pinMode(buzzerPin,OUTPUT);   //set buzzer pin as output
      Serial.begin(57600);
      Serial.println("Fastest Finger First Receiver");
  
      SPI.begin();
      radio.begin();
      network.begin(/*channel*/ 90, /*node address*/ this_node);
    }

void loop()
    {
      elapsedTime = millis();
      network.update();                  // Check the network regularly

     // Is there anything ready for us?
     while (network.available() )
       { // If so, grab it and print it out
         RF24NetworkHeader header;
         char messageToRecieve[32] = "";
         boolean recieve = false;
         while (!recieve)
            {
              recieve = network.read(header, messageToRecieve ,32);
              Serial.println(messageToRecieve);
            }  
                
              lastReceivedMessage_Time = elapsedTime;  // if any new message received keep time of that new message
              String player(messageToRecieve); // convert received message to string
               
              if(player == "1")  // if received message is 1 (from player 1)
                {
                   recievedMessage1_Time = elapsedTime;  // keep time this message in variable receivedMessage1_Time
                   Serial.print("P1: ");
                   Serial.println(recievedMessage1_Time); 
                }
              
              if(player == "2")  // if received message is 2 (from player 2)
                {
                   recievedMessage2_Time = elapsedTime;  // keep time this message in variable receivedMessage2_Time
                   Serial.print("P2: ");
                   Serial.println(recievedMessage2_Time);
                }
             
              if(player == "3") // if received message is 3 (from player 2)
                {
                   recievedMessage3_Time == elapsedTime;  // keep time of this message in variable receivedMessage3_Time
                   Serial.println("P3");
                }
             
              if(player == "4") // if received message is 4 (from player 4)
                {
                   recievedMessage4_Time == elapsedTime; // keep time of this message in variable receivedMessage4_Time
                   Serial.println("P4");
                }
               
              
                
                      
            }
            
            // after 2 seconds of last message received, make decision which player pressed the button first
            if(elapsedTime - lastReceivedMessage_Time == takeDecision)
                  { 
                    Serial.println("make decision"); 
                    Serial.print("elapsedTime: "); 
                    Serial.println(elapsedTime);
                    
                    //calculate time for each player after last message received
                    unsigned long player1 = elapsedTime - recievedMessage1_Time; 
                    unsigned long player2 = elapsedTime - recievedMessage2_Time;
                    unsigned long player3 = elapsedTime - recievedMessage3_Time;
                    unsigned long player4 = elapsedTime - recievedMessage4_Time;
                    
                    Serial.print("player1: "); 
                    Serial.println(player1);
                    Serial.print("player2: ");
                    Serial.println(player2);
                    Serial.print("player3: ");
                    Serial.println(player3);
                    
                    if((player1 > player2 || player2 > (takeDecision * noOfPlayers)) && (player1 > player3 || player3 > (takeDecision * noOfPlayers)) && (player1 > player4 || player4 > (takeDecision * noOfPlayers)) && (player1 <= (takeDecision * noOfPlayers)))
                      {  Serial.println("1");
                        beep();
                        u8g.firstPage();  
                        do 
                         { u8g.setFont(u8g_font_timB24);
                           u8g.drawStr(60,35,"1");
                         } while( u8g.nextPage() ); 
                 
                      } 
                    if((player2 > player1|| player1 > (takeDecision * noOfPlayers)) && (player2 > player3 || player3 > (takeDecision * noOfPlayers)) && (player2 > player4 || player4 > (takeDecision * noOfPlayers)) && (player2 <= (takeDecision * noOfPlayers)))
                      { Serial.println("2");
                        beep();
                        u8g.firstPage();  
                        do 
                         { u8g.setFont(u8g_font_timB24);
                           u8g.drawStr(60,35,"2");
                         } while( u8g.nextPage() ); 
                 
                      } 
                    if((player3 > player1 || player1 > (takeDecision * noOfPlayers)) && (player3 > player2 || player2 > (takeDecision * noOfPlayers)) && (player1 > player4 || player4 > (takeDecision * noOfPlayers)) && (player3 <= (takeDecision * noOfPlayers)))
                      { 
                        Serial.println("3");
                        beep();
                        u8g.firstPage();  
                        do 
                         { u8g.setFont(u8g_font_timB24);
                           u8g.drawStr(60,35,"3");
                         } while( u8g.nextPage() ); 
                 
                      } 
                    if((player4 > player1 || player1 > (takeDecision * noOfPlayers)) && (player4 > player2 || player2 > (takeDecision * noOfPlayers)) && (player4 > player3 || player3 > (takeDecision * noOfPlayers)) && (player4 <= (takeDecision * noOfPlayers)))
                      { Serial.println("4");
                        beep();
                        u8g.firstPage();  
                        do 
                         { u8g.setFont(u8g_font_timB24);
                           u8g.drawStr(60,35,"4");
                         } while( u8g.nextPage() ); 
                 
                      }   
                    delay(6000);
                } 
             
     }

//function to play beep sound on buzzer
void beep()
    {
      digitalWrite(buzzerPin, HIGH);
      delay(1000);
      digitalWrite(buzzerPin, LOW);
    }  
      
