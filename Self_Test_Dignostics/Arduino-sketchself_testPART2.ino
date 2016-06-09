#include "U8glib.h"
#include <IRremote.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define delay_val 2000
String TEST = "Test ";
String STARTED ="Started";
String FINISHED = "Finished";
String TESTED = "Tested";
String OK = "OK";
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI
 
RF24 radio(7,8);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t master_node = 0;

// Address of the other nodes
const uint16_t node_1 = 1;
const uint16_t node_2 = 2;
const uint16_t node_3 = 3;
const uint16_t node_4 = 4;
const uint16_t node_5 = 5;
const uint16_t node_6 = 6;
const uint16_t node_7 = 7;

boolean send_message = true;
//char messageToSend_1[32] = "NRFL2401 M1 TEST";  

char* messageToSend[]={"Module1 Test", "NRFL2401 M2 TEST", "NRFL2401 M3 TEST",
"NRFL2401 M4 TEST", "NRFL2401 M5 TEST","NRFL2401 M6 TEST" "NRFL2401 M7 TEST"};



byte Button1Pressed = 0;
byte Button1LastState = LOW;

byte touched = 0;
byte touchPadLastState=LOW;

byte RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;
byte recieved_code = 0;


void setup()
    { //Serial.begin(115200);
      u8g.setFont(u8g_font_timB14r);
    // u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
         
      irrecv.enableIRIn(); // Start the receiver 
      
    
     Serial.println("Self Dignostics");
     showMessageOnLcd(25,17,"Self Test",15,45,"Dignostics");
     delay(delay_val);
     
     Serial.begin(57600);
     Serial.println("NRF24L01 TEST  STARTED");
     showMessageOnLcd(5,17,"NRF24L01",1,45,TEST+STARTED);    
     delay(delay_val);
     SPI.begin();
     radio.begin();
     network.begin(/*channel*/ 90, /*node address*/ master_node);
    // while(25000>millis()){ nrf24l01_TEST();}
     
     button1_TEST(); 
     touchPad_TEST(); 
     IR_TEST();
     ldr_TEST();
     potentiometer_TEST();
     LM35_TEST();
             
   }
void loop()
    {      
     }

void showMessageOnLcd(int x,int y, String message1,int a,int b, String message2)
    {
   //   int stringLength1=message1.length()+1;
   //   int stringLength2=message2.length()+1;
   //   char char1[stringLength1];
   //   char char2[stringLength2];
      u8g.firstPage();
      do {//u8g.setFont(u8g_font_timB14);
            
           u8g.drawStr( x, y, message1.c_str());
           u8g.drawStr( a, b, message2.c_str());    
         } while( u8g.nextPage() );
    }

void button1_TEST()
    {  
      byte Button1 = 7;
      pinMode(Button1, INPUT);
      Serial.println("BUTTON 1 "+TEST+STARTED);
      showMessageOnLcd(1,17,"Button 1 "+TEST,5,45,STARTED);
      delay(delay_val);
      Serial.println("Press BUTTON1  3 Times");
      showMessageOnLcd(1,17,"Press Button1",5,45,"3 Times");
      delay(delay_val);
      while(40000>millis())
           {
            int Button1State = digitalRead(Button1);
     
            if(Button1State==HIGH && Button1LastState==LOW)
              {
               Button1Pressed++;
               Serial.print("button1 pressed:");
               Serial.println(Button1Pressed);
            /*   u8g.firstPage();
           do {  
                u8g.drawStr( 5, 30, "button1 pressed");
                u8g.setPrintPos(50, 55);
                u8g.print(Button1Pressed);
              } while( u8g.nextPage() );*/
              
                showMessageOnLcd(5,15,"button1 pressed",50,40,String(Button1Pressed));
              } 
            if(Button1Pressed == 3)
              {  
               Button1Pressed =0;
               Serial.println("BUTTON 1 "+TESTED+OK);
               showMessageOnLcd(1,17,"Button 1 "+TESTED,5,45,OK);
               delay(delay_val);
              }
        
            Button1LastState=Button1State;

            Button1State= digitalRead(Button1);    
          } 
        Serial.println("Button 1 "+TEST+FINISHED);
        showMessageOnLcd(1,17,"Button 1 "+TEST,5,45,FINISHED);
        delay(delay_val); 
       
        
    }  

void touchPad_TEST()
    { 
      byte touchpadPin = 4;
      pinMode(touchpadPin,INPUT);
      Serial.println("TOUCHPAD"+TEST+STARTED);
      showMessageOnLcd(1,17,"Touchpad "+TEST,5,45,STARTED);
      delay(delay_val);
      Serial.println("Touch TOUCHPAD 3 Times");
      showMessageOnLcd(1,17,"touch touchpad",5,45,"3 Times");
      delay(delay_val);
      while(66000 >millis())
           {
             int touchPadState = digitalRead(touchpadPin);
             if(touchPadState == HIGH && touchPadLastState == LOW)
               { 
                 touched++;
                 Serial.print("Touchpad Touched:");
                 Serial.println(touched);
               /*  u8g.firstPage();
                 do {  
                     u8g.drawStr( 5, 30, "touchpad touched");
                     u8g.setPrintPos(50, 55);
                     u8g.print(touched);
                    } while( u8g.nextPage() ); */
                  showMessageOnLcd(1,17,"Touchpad",5,45, "Touched   "+String(touched)); 
	        }
              if(touched == 3)
                {
                  touched = 0;
                  Serial.println("TOUCHPAD "+TESTED+OK);
                  showMessageOnLcd(1,17,"touchpad "+TESTED,5,45,OK);
                  delay(delay_val);
                 } 
        
               touchPadLastState=touchPadState;
 
               touchPadState= digitalRead(touchpadPin);    
              }
     
       Serial.println("TOUCHPAD " +TEST+FINISHED); 
       showMessageOnLcd(1,17,"Touchpad Test",5,45,"Finished");
       delay(delay_val);
    }     
   
void IR_TEST() 
    { String Code_Received = "Code Received";
      const unsigned long IR_CODE1 = 0xF20AC837;
      const unsigned long IR_CODE2 = 0xF20AA857;
      const unsigned long IR_CODE3 = 0xF20A28D7;
      const unsigned long IR_CODE4 = 0xF20A6897;
      Serial.println("IR RECIEVER "+TEST+STARTED);
      showMessageOnLcd(1,17,"IR RECIEVER",1,45,TEST+STARTED);
      delay(delay_val);
      Serial.println("PRESS REMOTE BUTTONS 1,2,3,4");
      showMessageOnLcd(1,17,"Press remote ",1,45,"Buttons 1,2,3,4");
      delay(delay_val);
      while(90000>millis()) 
           {
             if(irrecv.decode(&results)) 
               {
                 //translateIR();
                 switch(results.value)
                       {
                         case IR_CODE1: Serial.println(Code_Received);showMessageOnLcd(1,15,Code_Received,5,45,"");recieved_code++; break;
                         case IR_CODE2: Serial.println(Code_Received);showMessageOnLcd(1,30,Code_Received,5,45,"");recieved_code++; break;
                         case IR_CODE3: Serial.println(Code_Received);showMessageOnLcd(1,45,Code_Received,5,45,"");recieved_code++; break;
                         case IR_CODE4: Serial.println(Code_Received);showMessageOnLcd(1,60,Code_Received,5,45,"");recieved_code++; break;
                         delay(500);
                       }
                 irrecv.resume(); // receive the next value      
                 if (recieved_code==4){recieved_code=0; Serial.println("IR RECEIVER "+TESTED+OK);  showMessageOnLcd(1,17,"IR "+TESTED,5,45,OK);}
                   
                }
             }
      Serial.println("IR RECIEVER "+TEST+FINISHED);
      showMessageOnLcd(1,17,"IR RECIEVER ",1,45,TEST+FINISHED);
      delay(delay_val);
    }
  
      
 void ldr_TEST()
    {  
      Serial.println("LDR "+TEST+STARTED);
      showMessageOnLcd(22,17,"LDR "+TEST,20,45,STARTED);
      delay(delay_val);
      Serial.println("SHINE LIGHT ON LDR");
      showMessageOnLcd(22,17,"Shine light",30,45,"ON LDR");
      delay(delay_val);
      //unsigned int time = millis();
      while(116000 > millis())
           {
             int ldr_Value = analogRead(A0);
             Serial.print("LDR Value: ");
             Serial.println(ldr_Value);
           /*  u8g.firstPage();
           do {  
                u8g.drawStr( 20, 30, "LDR Value");
                u8g.setPrintPos(50, 55);
                u8g.print(ldr_Value);
              } while( u8g.nextPage() );*/
            showMessageOnLcd(22,15,"LDR Value",50,40,String(ldr_Value)); 
             
           }
      //Serial.print(time);     
      Serial.println("LDR "+TEST+FINISHED); 
      showMessageOnLcd(5,17,"LDR "+TEST,18,45,FINISHED);
      delay(delay_val);
     
    } 
void potentiometer_TEST()
    {  
      Serial.println("POTENTIOMETER "+TEST+STARTED);
      showMessageOnLcd(1,17,"Potentiometer",1,45,TEST+STARTED);
      delay(delay_val);  
      Serial.println("Turn knob of Potentiometer Knob");
      showMessageOnLcd(1,20,"Turn knob of",1,40,"Potentiometer");
      delay(delay_val);
      
      while(142000 > millis())
          {
            int pot_Value = analogRead(A1);
            Serial.print("Potentiometer Value: ");
            Serial.println(pot_Value);
           /* u8g.firstPage();
           do {  
                u8g.drawStr( 30, 30, "Pot Value");
                u8g.setPrintPos(50, 55);
                u8g.print(pot_Value);
              } while( u8g.nextPage() );*/
             showMessageOnLcd(20,15,"Pot Value",50,40,String(pot_Value));  
            
           }
       Serial.println("POTENTIOMETER "+TEST+FINISHED);
       showMessageOnLcd(5,17,"Potentiometer",1,45,TEST+FINISHED);
       delay(delay_val);
      
     
    } 
    
  void LM35_TEST()
      { 
        Serial.println("LM35 "+TEST+STARTED);
        showMessageOnLcd(1,17,"LM35 "+TEST,5,45,STARTED);
        delay(delay_val);
        Serial.println("Increase OR Decrease the Temperature");
        showMessageOnLcd(1,17,"increase or",5,45,"decrease temp");
        delay(delay_val);
        while(168000>millis()) 
             {
               float temperature =( 5.0 * analogRead(A3) * 100.0) / 1024.0;
               Serial.print("temperature: ");
               Serial.print(temperature);
               Serial.print(char(176)); 
               Serial.println("C");
               u8g.firstPage();
               do {  
                    u8g.drawStr( 5, 15, "Temperature");
                    u8g.setPrintPos(30, 40);
                    u8g.print(temperature);
                    u8g.setPrintPos(75, 40);
                    u8g.print(char(176));
                    u8g.drawStr( 80, 40, "C");
                  } while( u8g.nextPage() );
           
               delay(500);
             }
         Serial.println("LM35"+TEST+FINISHED);
         showMessageOnLcd(1,17,"LM35 "+TEST,5,45,FINISHED);
         delay(delay_val);
      }
             

 
 
 void nrf24l01_TEST() 
    {
       // Pump the network regularly
       network.update();
      if(send_message)  // if there is typed message on serial monitor ready to send
        {
         for(int i=1; i<8; i++)
            {
              RF24NetworkHeader header1(/*to node*/ i);
              boolean message = network.write(header1, messageToSend[i-1], 32); // send message to other user
              if(message)
                {
                 // Serial.print("Sender: ");  // print message on serial monitor
                 Serial.println(messageToSend[i-1]);
                 u8g.firstPage();
               do { 
                    u8g.setPrintPos(1, 30);
                    u8g.print(messageToSend[i-1]);
                  } while( u8g.nextPage() );           
                }
              else
                 {
                  Serial.print("could not write to module ");  // if it is failed to send message prompt error
                  Serial.println(i);
               /*   u8g.firstPage();
               do { u8g.drawStr(1,15,"could not write");
                    u8g.drawStr(5,45,"to module");
                    u8g.setPrintPos(100, 45);
                    u8g.print(i);
                  } while( u8g.nextPage() );*/
                  showMessageOnLcd(1,17,"could not write",5,45,"to module"+String(i));
                 } delay(1000); 
             }
           send_message = false;
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
              //Serial.print("Reciver: ");   // print recived data on serial monitor
              Serial.println(messageToRecieve);
              u8g.firstPage();
               do { 
                    u8g.setPrintPos(1, 30);
                    u8g.print(messageToRecieve);
                  } while( u8g.nextPage() );
            }
 
        }

 }   
                               
