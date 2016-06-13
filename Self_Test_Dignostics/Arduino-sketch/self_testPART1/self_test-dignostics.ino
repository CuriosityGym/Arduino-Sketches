#include <Adafruit_NeoPixel.h>
#include "U8glib.h"
#include <SPI.h>
#include <SD.h>
#include <IRremote.h>
#include <ELClient.h>
#include <ELClientRest.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define PIN 6
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

char* messageToSend[]={"Module1 Test", "Module2 Test", "Module3 Test",
"Module4 Test", "Module5 Test","Module6 Test" "Module7 Test"};

int led_delay= 1000;
int ws2812_delay =1000;
int sd_delay = 3000;

byte Button1Pressed = 0;
int Button1LastState = LOW;

byte touched = 0;
int touchPadLastState=LOW;

int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;
byte recieved_code = 0;

char buff[32];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

ELClient esp(&Serial, &Serial);

// Initialize a REST client on the connection to esp-link
ELClientRest rest(&esp);

boolean wifiConnected = false;

// Callback made from esp-link to notify of wifi status changes
// Here we print something out and set a global flag
void wifiCb(void *response) 
    {
      ELClientResponse *res = (ELClientResponse*)response;
      if (res->argc() == 1) 
         {
           uint8_t status;
           res->popArg(&status, 1);

           if(status == STATION_GOT_IP) 
             {
               Serial.println("WIFI CONNECTED");  //Wifi gets connected at this place
               wifiConnected = true;
             } 
           else 
             {
               Serial.print("WIFI NOT READY: ");//Wifi not connected,check connection
               Serial.println(status);
               wifiConnected = false;
             }
         }
    }
    
void setup()
    { Serial.begin(115200);
      u8g.setFont(u8g_font_timB14);
      u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
         
      irrecv.enableIRIn(); // Start the receiver 
      strip.begin();
      strip.show(); // Initialize all pixels to 'off'                                       //
    
    
     Serial.println("Self Test Dignostics");
     showMessageOnLcd(25,17,"Self Test",15,45,"Dignostics");
     delay(2000);
     

    
   // 
    // Serial.begin(115200);
     
    ldr_TEST();
    potentiometer_TEST();
    LM35_TEST();
     button1_TEST(); 
    touchPad_TEST(); 
    IR_TEST();
    
     
    LED13_TEST();
    LED_TEST();
    WS2812_TEST();
    buzzer_TEST();  
     sdCard_TEST();
    esp8266_TEST();
     Serial.println("NRF24L01 TEST  STARTED");
    showMessageOnLcd(5,17,"NRF24L01",1,45,"Test Started");    
    delay(1000);
    Serial.begin(57600);
    SPI.begin();
    radio.begin();
    network.begin(/*channel*/ 90, /*node address*/ master_node);
    while(240000>millis()){nrf24l01_TEST();}
     Serial.println("SELF TEST DIGNOSTICS FINISHED");
     showMessageOnLcd(25,17,"Self Test",1,45,"Dignostics Finished");   
    }   
    
void loop()
    { 
     
       
     }

void showMessageOnLcd(int x,int y, const char* message1,int a,int b, const char* message2)
    {u8g.firstPage();
      do { //u8g.setFont(u8g_font_timB14);
           u8g.drawStr( x, y, message1);
           u8g.drawStr( a, b, message2);    
         } while( u8g.nextPage() );
    }

void LED13_TEST()
    { 
      byte led13 = 13;
      pinMode(led13,OUTPUT);   
      Serial.println("LED13 TEST STARTED");
      showMessageOnLcd(30,17,"LED13 ",1,45,"Test Started");
      delay(3000);   
      for(byte a = 0; a<5;a++)
           {
             digitalWrite(led13,HIGH); delay(1000);
             digitalWrite(led13,LOW); delay(1000);
           }  
      Serial.println("LED13 TEST FINISHED");
       showMessageOnLcd(5,17,"LED13 TEST ",1,45,"FINISHED");
      delay(2000); 
    }
  
void LED_TEST()
    {     
     int led_Pins[] = {1,2,3,4,5,6,7,8,9,10,11,12};   //an array of pin numbers to which LEDs are attached
     int pin_Count = 12;   // the number of pins (i.e. the length of the array)
      for( int thisPin = 0; thisPin < pin_Count; thisPin++) // set all LEDs to output
         {
           pinMode(led_Pins[thisPin],OUTPUT);
         } 
      Serial.println("LED TEST STARTED");
      showMessageOnLcd(5,17,"LED TEST",22,45,"STARTED");
      delay(2000);   
      for(int i=0; i<12; i++)
         {
           digitalWrite(led_Pins[i],HIGH);
           Serial.print("LED: ");
           Serial.print(i+1);
           Serial.println(" ON");
           u8g.firstPage();
           do {  
                u8g.drawStr( 10, 30, "LED ");
                u8g.setPrintPos(58, 30);
                u8g.print(i+1);
                u8g.drawStr( 90, 30, "ON");
              } while( u8g.nextPage() );
           delay(led_delay);
         }
      delay(1000);
      for(int j=11; j>=0; j--)
         {
           digitalWrite(led_Pins[j],LOW);
           Serial.print("LED: ");
           Serial.print(j+1);
           Serial.println(" OFF");
           u8g.firstPage();
           do {  
               u8g.drawStr( 10, 30, "LED ");
               u8g.setPrintPos(58, 30);
               u8g.print(j+1);
               u8g.drawStr( 90, 30, "OFF");
              } while( u8g.nextPage() );
           delay(led_delay);
         }
      delay(1000);   
      Serial.println("LED TEST  FINISHED");
       showMessageOnLcd(5,17,"LED TEST ",18,45,"FINISHED");
      delay(2000); 
    }
  
void WS2812_TEST()
   { 
     Serial.println("WS2812 TEST STARTED");
     showMessageOnLcd(1,17,"WS2812 TEST",5,45,"STARTED");
     delay(2000);
     for(int i=0; i<3; i++)
        {
          switch(i)
               { 
                case 0: strip.setPixelColor(0,255,0,0);strip.show();
                Serial.println("Red Color");
                showMessageOnLcd(18,17,"Pure RED",35,45,"Color");
                delay(ws2812_delay);break;
                case 1: strip.setPixelColor(0,0,255,0);strip.show();
                Serial.println("Green Color");
                showMessageOnLcd(5,17,"Pure GREEN",35,45,"Color");
                delay(ws2812_delay);break;
                case 2: strip.setPixelColor(0,0,0,255);strip.show();
                Serial.println("Blue Color");
                showMessageOnLcd(12,17,"Pure BLUE",35,45,"Color");
                delay(ws2812_delay); break;
               }
         } 
    delay(2000);
    Serial.println("Rainbow Colors");
    showMessageOnLcd(25,17,"Rainbow",35,45,"Colors");
    delay(2000); 

     for(int j=0; j<256; j++) 
        {
          for(byte i=0; i<strip.numPixels(); i++) 
             {
               strip.setPixelColor(i, Wheel((i+j) & 255));
             }
          strip.show();
          delay(20);
        } 
      delay(1000);  
  
    Serial.println("FADE IN FADE OUT EFFECT");
    showMessageOnLcd(1,17,"Fade In Fade ",15,45,"Out Effect");
    delay(3000);
    for(int j = 0; j < 3; j++ ) 
       { // Fade IN
       for(int k = 0; k < 256; k++)
          { 
           switch(j)  
                 { 
                   case 0: strip.setPixelColor(0,k,0,0);strip.show(); break;
                   case 1: strip.setPixelColor(0,0,k,0);strip.show(); break;
                   case 2: strip.setPixelColor(0,0,0,k);strip.show(); break; 
                  }
           delay(3);
          }
         // Fade OUT
       for(int k = 255; k >= 0; k--) 
          { 
           switch(j) 
                 { 
                   case 0: strip.setPixelColor(0,k,0,0);strip.show(); break;
                   case 1: strip.setPixelColor(0,0,k,0);strip.show(); break;
                   case 2: strip.setPixelColor(0,0,0,k);strip.show(); break;
                  }
           delay(3);  
           }
           
       }
     delay(1000);  
     Serial.println("WS2812 TEST FINISHED");
     showMessageOnLcd(1,17,"WS2812 TEST",1,45,"FINISHED");
     delay(2000); 
   } 
   
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
   
   void buzzer_TEST()
    { Serial.println("BUZZER TEST STARTED");
      showMessageOnLcd(0,17,"BUZZER TEST",10,45,"STARTED");
      delay(1000);
     // notes in the melody:
     int melody[]= {196, 196, 220, 196, 262, 247, 196, 196, 220, 196, 294, 262, 
                    196, 196, 392, 330, 262, 247, 220, 349, 349, 330, 262, 294, 262 };
     // note durations: 4 = quarter note, 8 = eighth note, etc.:
     int noteDurations[] = { 8,8,4,4,4,2,8,8,4,4,4,2,8,8,4,4,4,4,3,8,8,4,4,4,2 };

     // iterate over the notes of the melody: 
     for(int thisNote = 0; thisNote < 26; thisNote++) 
        {
         // to calculate the note duration, take one second 
         // divided by the note type.
         //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
         int noteDuration = 1000/noteDurations[thisNote];
         tone(A2, melody[thisNote],noteDuration);
         // to distinguish the notes, set a minimum time between them.
         // the note's duration + 30% seems to work well:
         int pauseBetweenNotes = noteDuration * 1.60;
         delay(pauseBetweenNotes);
         // stop the tone playing:
         noTone(8);
        }
      Serial.println("BUZZER TEST FINISHED"); 
      showMessageOnLcd(0,17,"BUZZER TEST",5,45,"FINISHED");
      delay(2000); 
    }
   
void sdCard_TEST()
    {  byte chipSelect = 6;   //cs pin of SD card shield              
       // Open serial communications and wait for port to open:
     
      Serial.println("SD CARD TEST STARTED");
      showMessageOnLcd(1,17,"SDCARD Test",5,45,"Started");
      delay(sd_delay);
      while (!Serial)
          {
            ; // wait for serial port to connect. Needed for native USB port only
          }
       
      Serial.print("Initializing SD card...");
      showMessageOnLcd(1,17,"Initializing",5,45,"SD Card...");
     // see if the card is present and can be initialized:
     if (!SD.begin(chipSelect)) 
        {
          Serial.println("Card failed, or not present");
          showMessageOnLcd(1,17,"Card Failed",1,45,"or not present");
          delay(sd_delay);
          // don't do anything more:
          return;
        }
     Serial.println("card initialized.");
     showMessageOnLcd(15,17,"Card ",1,45,"Initialized");
     delay(sd_delay);
    
     File dataFile = SD.open("sdTest.txt", FILE_WRITE);
     if(dataFile) 
       {
         Serial.print("Writing to sdTest.txt...");
         showMessageOnLcd(1,17,"Writing to",1,45,"sdTest.txt...");
         delay(sd_delay);
         dataFile.println("The quick brown fox jumps over the lazy dog");
         // close the file:
         dataFile.close();
         Serial.println("done.");
         showMessageOnLcd(1,17,"Writing to",1,45,"sdTest.txt Done");
         delay(1000);
       } 
     else 
       {
         // if the file didn't open, print an error:
         Serial.println("error opening sdTest.txt");
         showMessageOnLcd(1,17,"Error opening",1,45,"sdTest.txt");
         delay(sd_delay);
       }

     // re-open the file for reading:
     dataFile = SD.open("sdTest.txt");
     if(dataFile)
       {
        Serial.println("sdTest.txt:");
        showMessageOnLcd(1,17,"reading ",1,45,"from sdCard");
        delay(sd_delay);
        // read from the file until there's nothing else in it:
        while(dataFile.available()) 
             {
               Serial.write(dataFile.read());
               showMessageOnLcd(1,17,"successfully",0,45,"read from Card");
             }
        // close the file:
        dataFile.close();
        delay(1000);
       } 
     else 
       {
        // if the file didn't open, print an error:
        Serial.println("error opening sdTest.txt");
        showMessageOnLcd(1,17,"error opening",1,45,"sdTest.txt");
        delay(sd_delay);
       } 
      
      SD.remove("sdTest.txt"); 
    Serial.println("SD CARD TEST FINISHED"); 
    showMessageOnLcd(1,17,"SDCard Test",1,45,"FINISHED"); 
    delay(sd_delay);   
  }    


void button1_TEST()
    {  
      byte Button1 = 7;
      pinMode(Button1, INPUT);
      Serial.println("BUTTON 1 TEST STARTED");
      showMessageOnLcd(1,17,"Button 1 Test",5,45,"Started");
      delay(2000);
      Serial.println("Press BUTTON1  3 Times");
      showMessageOnLcd(1,17,"Press Button1",5,45,"3 Times");
      delay(2000);
      while(73000>millis() && Button1Pressed < 3)
           {
            int Button1State = digitalRead(Button1);
     
            if(Button1State==HIGH && Button1LastState==LOW)
              {
               Button1Pressed++;
               Serial.print("button1 pressed:");
               Serial.println(Button1Pressed);
               u8g.firstPage();
           do {  
                u8g.drawStr( 5, 30, "button1 pressed");
                u8g.setPrintPos(50, 55);
                u8g.print(Button1Pressed);
              } while( u8g.nextPage() );
              } 
            if(Button1Pressed == 3)
              {  
              // Button1Pressed =0;
               Serial.println("BUTTON 1 TESTED OK");
               showMessageOnLcd(1,17,"button 1 tested",5,45,"OK");
               delay(2000);
              }
        
            Button1LastState=Button1State;

            Button1State= digitalRead(Button1);    
          } 
        Serial.println("Button 1 TEST IS FINISHED");
        showMessageOnLcd(1,17,"Button 1 Test",5,45,"Is Finished");
        delay(2000); 
       
        
    }  

void touchPad_TEST()
    { 
      byte touchpadPin = 4;
      pinMode(touchpadPin,INPUT);
      Serial.println("TOUCHPAD TEST STARTED");
      showMessageOnLcd(1,17,"Touchpad Test",5,45,"Started");
      delay(2000);
      Serial.println("Press TOUCHPAD 3 Times");
      showMessageOnLcd(1,17,"Touch Touchpad",5,45,"3 Times");
      delay(2000);
      while(94000 >millis() && touched < 3)
           {
             int touchPadState = digitalRead(touchpadPin);
             if(touchPadState == HIGH && touchPadLastState == LOW)
               { 
                 touched++;
                 Serial.print("touchPad Touched:");
                 Serial.println(touched);
                 u8g.firstPage();
                 do {  
                     u8g.drawStr( 5, 30, "touchpad touched");
                     u8g.setPrintPos(50, 55);
                     u8g.print(touched);
                    } while( u8g.nextPage() );
	        }
              if(touched == 3)
                {
                 // touched = 0;
                  Serial.println("TOUCHPAD TESTED OK");
                  showMessageOnLcd(1,17,"touchpad tested",5,45,"OK");
                  delay(2000);
                 } 
        
               touchPadLastState=touchPadState;
 
               touchPadState= digitalRead(touchpadPin);    
              }
     
       Serial.println("TOUCHPAD TEST FINISHED"); 
       showMessageOnLcd(1,17,"Touchpad Test",5,45,"Finished");
       delay(2000);
    }     
   
void IR_TEST() 
    { 
      const unsigned long IR_CODE1 = /*0xFFFFFFFF; */0xF20AC837;
      const unsigned long IR_CODE2 = /*0x7200000;   */0xF20AA857;
      const unsigned long IR_CODE3 = /*0x897580E3;   */0xF20A28D7;
      const unsigned long IR_CODE4 = /*0x6C042472;   */0xF20A6897;
      Serial.println("IR RECIEVER TEST STARTED");
      showMessageOnLcd(1,17,"IR RECIEVER",1,45,"Test Started");
      delay(2000);
      Serial.println("PRESS REMOTE BUTTONS 1,2,3,4");
      showMessageOnLcd(1,17,"Press remote ",1,45,"Buttons 1,2,3,4");
      delay(2000);
      while(115000>millis() && recieved_code < 4) 
           {
             if(irrecv.decode(&results)) 
               {
                 //translateIR();
                 switch(results.value)
                       {
                         case IR_CODE1: Serial.println("CODE 1 RECIEVED");showMessageOnLcd(1,17,"IR CODE 1",5,45,"RECIEVED");recieved_code++; break;
                         case IR_CODE2: Serial.println("CODE 2 RECIEVED");showMessageOnLcd(1,17,"IR CODE 2",5,45,"RECIEVED");recieved_code++; break;
                         case IR_CODE3: Serial.println("CODE 3 RECIEVED");showMessageOnLcd(1,17,"IR CODE 3",5,45,"RECIEVED");recieved_code++; break;
                         case IR_CODE4: Serial.println("CODE 4 RECIEVED");showMessageOnLcd(1,17,"IR CODE 4",5,45,"RECIEVED");recieved_code++; break;
                         delay(500);
                       }
                 irrecv.resume(); // receive the next value      
                 if(recieved_code==4)
                   { 
                     Serial.println("IR RECEIVER TESTED OK"); 
                     showMessageOnLcd(1,17,"IR tested",5,45,"OK");
                     delay(2000);
                   }
                   
                 }
             }
      Serial.println("IR RECIEVER TEST FINISHED");
      showMessageOnLcd(1,17,"IR RECIEVER ",1,45,"Test Finished");
      delay(2000);
    }
  
      
 void ldr_TEST()
    {  
      Serial.println("LDR TEST STARTED");
      showMessageOnLcd(22,17,"LDR TEST",5,45,"STARTED");
      delay(2000);
      Serial.println("SHINE LIGHT ON LDR");
      showMessageOnLcd(22,17,"Shine light",30,45,"ON LDR");
      delay(2000);
      //unsigned int time = millis();
      while(20000 > millis())
           {
             int ldr_Value = analogRead(A0);
             Serial.print("LDR Value: ");
             Serial.println(ldr_Value);
             u8g.firstPage();
           do {  
                u8g.drawStr( 20, 30, "LDR Value");
                u8g.setPrintPos(50, 55);
                u8g.print(ldr_Value);
              } while( u8g.nextPage() );
            delay(500); 
           }
      //Serial.print(time);     
      Serial.println("LDR TEST FINISHED"); 
      showMessageOnLcd(5,17,"LDR TEST ",18,45,"FINISHED");
      delay(2000);
     
    } 
void potentiometer_TEST()
    {  
      Serial.println("POTENTIOMETER TEST STARTED");
      showMessageOnLcd(1,17,"Potentiometer",1,45,"Test Started");
      delay(2000);  
      Serial.println("Turn the Potentiometer Knob");
      showMessageOnLcd(1,20,"Turn knob of",1,40,"Potentiometer");
      delay(2000);
      
      while(31000 > millis())
          {
            int pot_Value = analogRead(A1);
            Serial.print("Potentiometer Value: ");
            Serial.println(pot_Value);
            u8g.firstPage();
           do {  
                u8g.drawStr( 30, 30, "Pot Value");
                u8g.setPrintPos(50, 55);
                u8g.print(pot_Value);
              } while( u8g.nextPage() );
            delay(500);
           }
       Serial.println("POTENTIOMETER TEST FINISHED");
       showMessageOnLcd(5,17,"Potentiometer",1,45,"test  finished");
       delay(2000);
      
     
    } 
    
  void LM35_TEST()
      { 
        Serial.println("LM35 TEST STARTED");
        showMessageOnLcd(1,17,"LM35 TEST",5,45,"STARTED");
        delay(2000);
        Serial.println("Increase OR Decrease the Temperature");
        showMessageOnLcd(1,17,"increase or",5,45,"decrease temp");
        delay(2000);
        while(52000>millis()) 
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
         Serial.println("LM35 TEST FINISHED");
         showMessageOnLcd(1,17,"LM35 Test",5,45,"Finished");
         delay(2000);
      }
             
   
void esp8266_setup()
    { 
       // Serial.begin(115200);   // the baud rate here needs to match the esp-link config
      Serial.println("EL-Client starting!");
      // Sync-up with esp-link, this is required at the start of any sketch and initializes the
      // callbacks to the wifi status change callback. The callback gets called with the initial
      // status right after Sync() below completes.
      esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
      bool ok;
      do 
       {
         ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
         if (!ok) Serial.println("EL-Client sync failed!"); showMessageOnLcd(20,15, "EL-Client",1,45, "sync failed!");
       } while(!ok);
      Serial.println("EL-Client synced!");

      // Get immediate wifi status info for demo purposes. This is not normally used because the
      // wifi status callback registered above gets called immediately. 
      esp.GetWifiStatus();
      ELClientPacket *packet;
      if((packet=esp.WaitReturn()) != NULL) 
        {
          Serial.print("Wifi status: ");
          Serial.println(packet->value);
        }

      // Set up the REST client to talk to www.timeapi.org, this doesn't connect to that server,
      // it just sets-up stuff on the esp-link side
      int err = rest.begin("curiositygym.com");
      if(err != 0) 
        {
          Serial.print("REST begin failed: ");
          Serial.println(err);
          while(1) ;
        }
      Serial.println("EL-REST ready");
    }   
void esp8266_TEST() 
    { 
      showMessageOnLcd(20,15, "ESP8266",5,45, "Test Started");
      delay(2000); 
      
      esp8266_setup();
      
      sprintf(buff, "/IdIoTWareShield_ESP_test.html");
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[20];
          uint16_t code = rest.waitResponse(response, 20);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println("ARDUINO: GET successful:");
             Serial.println(response);
             u8g.firstPage();
           do {  
                u8g.setPrintPos(10, 30);
                u8g.print(response);
              } while( u8g.nextPage() );
           delay(2000);   
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             showMessageOnLcd(20,15, "ARDUINO:",1,45, "GET failed:");
             Serial.println(code);
            }
          delay(3000);
        }
       showMessageOnLcd(20,15, "ESP8266",1,45, "Test Finished");
       delay(2000); 
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
                  u8g.firstPage();
               do { u8g.drawStr(1,15,"could not write");
                    u8g.drawStr(5,45,"to module");
                    u8g.setPrintPos(100, 45);
                    u8g.print(i);
                  } while( u8g.nextPage() );
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
                     
