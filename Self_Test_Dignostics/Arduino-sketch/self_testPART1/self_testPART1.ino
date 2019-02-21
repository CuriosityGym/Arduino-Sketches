#include <Adafruit_NeoPixel.h>
#include "U8glib.h"
#include <SPI.h>
#include <SD.h>
#include <ELClient.h>
#include <ELClientRest.h>
#define delay_val 2000
#define delay_val_1k 200
#define PIN 6
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI

String TEST="Test ";
String FINISHED="Finished";
String STARTED="Started";
String FILENAME="sdTest.txt";
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
              // Serial.println("WIFI CONNECTED");  //Wifi gets connected at this place
               wifiConnected = true;
             } 
           else 
             {
               //Serial.print("WIFI NOT READY: ");//Wifi not connected,check connection
               Serial.println(status);
               wifiConnected = false;
             }
         }
    }
    
void setup()
    { 
      Serial.begin(115200);
      u8g.setFont(u8g_font_timB14);
      u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
        
      strip.begin();
      strip.show(); // Initialize all pixels to 'off'                                       //
    
    
     //Serial.println("Self"+TEST+" Dignostics");
     showMessageOnLcd(25,17,"Self "+TEST,15,45,"Dignostics");
     delay(delay_val);
      
    //sdCard_TEST(); 
   
    LED13_TEST();
    LED_TEST();
    WS2812_TEST();
    buzzer_TEST();   
    esp8266_TEST(); 
  
        
    }
void loop()
    {      
     }

void showMessageOnLcd(int x,int y, String message1,int a,int b, String message2)
    {
      int stringLength1=message1.length()+1;
      int stringLength2=message2.length()+1;
      char char1[stringLength1];
      char char2[stringLength2];
      u8g.firstPage();
      do { //u8g.setFont(u8g_font_timB14);
            
           u8g.drawStr( x, y, message1.c_str());
           u8g.drawStr( a, b, message2.c_str());    
         } while( u8g.nextPage() );
    }

void LED13_TEST()
    { 
      byte led13 = 13;
      pinMode(led13,OUTPUT);   
      Serial.println("LED13 "+TEST+STARTED);
      showMessageOnLcd(30,17,"LED13 ",1,45,TEST+STARTED);
      delay(delay_val);   
      for(byte a;a<5;a++)
      {
             digitalWrite(led13,HIGH);
             delay(delay_val_1k);
             digitalWrite(led13,LOW);
             delay(delay_val_1k);
       }  
      Serial.println("LED13 "+TEST+FINISHED);
       showMessageOnLcd(5,17,"LED13 "+TEST ,1,45,FINISHED);
      delay(delay_val); 
    }
  
void LED_TEST()
    {     
    // int led_Pins[] = {1,2,3,4,5,6,7,8,9,10,11,12};   //an array of pin numbers to which LEDs are attached
     byte pin_Count = 12;   // the number of pins (i.e. the length of the array)
        for( byte thisPin = 0; thisPin < pin_Count; thisPin++) // set all LEDs to output
         {
           pinMode(thisPin,OUTPUT);
         } 
      Serial.println("LED "+TEST+STARTED);
      showMessageOnLcd(5,17,"LED "+TEST,22,45,STARTED);
      delay(delay_val);   
      for(byte i=0; i<pin_Count; i++)
         {
           digitalWrite(i,HIGH);
           Serial.print("LED: ");
           Serial.print(i);
           Serial.println(" ON");
           /*u8g.firstPage();
           do {  
                u8g.drawStr( 10, 30, "LED ");
                u8g.setPrintPos(58, 30);
                u8g.print(i+1);
                u8g.drawStr( 90, 30, "ON");
              } while( u8g.nextPage() );*/
           showMessageOnLcd(10, 30,"LED "+ String(i),90,30,"ON");   
           delay(delay_val_1k);
         }
      delay(delay_val);
      for(byte j=pin_Count; j>0; j--)
         {
           digitalWrite(j,LOW);
           Serial.print("LED: ");
           Serial.print(j);
           Serial.println(" OFF");
          /* u8g.firstPage();
           do {  
               u8g.drawStr( 10, 30, "LED ");
               u8g.setPrintPos(58, 30);
               u8g.print(j+1);
               u8g.drawStr( 90, 30, "OFF");
              } while( u8g.nextPage() );*/
           showMessageOnLcd(10, 30,"LED "+String(j+1),90,30,"OFF");    
           delay(delay_val_1k);
           if(j==0) break;
         }
      delay(delay_val);   
      Serial.println("LED "+TEST+FINISHED);
      showMessageOnLcd(5,17,"LED "+TEST,18,45,FINISHED);
      delay(delay_val); 
    }
  
void WS2812_TEST()
   { 
     Serial.println("WS2812 "+TEST+STARTED);
     showMessageOnLcd(1,17,"WS2812 "+TEST,5,45,STARTED);
     delay(delay_val);
     for(byte i=0; i<3; i++)
        {
          switch(i)
               { 
                case 0: strip.setPixelColor(0,255,0,0);strip.show();
                Serial.println("Red Color");
                showMessageOnLcd(18,17,"Pure RED",35,45,"Color");
                delay(delay_val);break;
                case 1: strip.setPixelColor(0,0,255,0);strip.show();
                Serial.println("Green Color");
                showMessageOnLcd(5,17,"Pure GREEN",35,45,"Color");
                delay(delay_val);break;
                case 2: strip.setPixelColor(0,0,0,255);strip.show();
                Serial.println("Blue Color");
                showMessageOnLcd(12,17,"Pure BLUE",35,45,"Color");
                delay(delay_val); break;
               }
         } 
    delay(delay_val);
    Serial.println("Rainbow Colors");
    showMessageOnLcd(25,17,"Rainbow",35,45,"Colors");
    delay(delay_val); 

     for(int j=0; j<256; j++) 
        {
          for(byte i=0; i<strip.numPixels(); i++) 
             {
               strip.setPixelColor(i, Wheel((i+j) & 255));
             }
          strip.show();
          delay(20);
        } 
      delay(delay_val);  
  
    Serial.println("FADE IN FADE OUT EFFECT");
    showMessageOnLcd(1,17,"Fade In Fade ",15,45,"Out Effect");
    delay(delay_val);
    for(byte j = 0; j < 3; j++ ) 
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
       for(byte k = 255; k >= 0; k--) 
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
     Serial.println("WS2812 "+TEST+FINISHED);
     showMessageOnLcd(1,17,"WS2812 "+TEST,1,45,FINISHED);
     delay(delay_val); 
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
    { Serial.println("BUZZER "+TEST+STARTED);
      showMessageOnLcd(0,17,"BUZZER "+TEST,10,45,STARTED);
      delay(delay_val);
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
      Serial.println("BUZZER "+TEST+FINISHED); 
      showMessageOnLcd(0,17,"BUZZER "+TEST,5,45,FINISHED);
      delay(delay_val); 
    }
   
void sdCard_TEST()
    {  byte chipSelect = 6;   //cs pin of SD card shield              
       // Open serial communications and wait for port to open:
     
      Serial.println("SD CARD "+TEST+STARTED);
      showMessageOnLcd(1,17,"SDCARD "+TEST,5,45,STARTED);
      delay(delay_val);
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
          delay(delay_val);
          // don't do anything more:
          return;
        }
     Serial.println("card initialized.");
     showMessageOnLcd(15,17,"Card ",1,45,"Initialized");
     delay(delay_val);
    
     File dataFile = SD.open(FILENAME, FILE_WRITE);
     if(dataFile) 
       {
         Serial.print("Writing to "+FILENAME);
         showMessageOnLcd(1,17,"Writing to",1,45,FILENAME);
         delay(delay_val);
         dataFile.println("The quick brown fox jumps over the lazy dog");
         // close the file:
         dataFile.close();
         Serial.println("done.");
         showMessageOnLcd(1,17,"Writing to",1,45,FILENAME+" Done");
         delay(delay_val);
       } 
     else 
       {
         // if the file didn't open, print an error:
         Serial.println("error opening " +FILENAME);
         showMessageOnLcd(1,17,"Error opening",1,45,FILENAME);
         delay(delay_val);
       }

     // re-open the file for reading:
     dataFile = SD.open(FILENAME);
     if(dataFile)
       {
        Serial.println(FILENAME);
        showMessageOnLcd(1,17,"reading ",1,45,"from sdCard");
        delay(delay_val);
        // read from the file until there's nothing else in it:
        while(dataFile.available()) 
             {
               Serial.write(dataFile.read());
               showMessageOnLcd(1,17,"successfully",0,45,"read from Card");
             }
        // close the file:
        dataFile.close();
        delay(delay_val);
       } 
     else 
       {
        // if the file didn't open, print an error:
        Serial.println("error opening sdTest.txt");
        showMessageOnLcd(1,17,"error opening",1,45,"sdText.txt");
        delay(delay_val);
       } 
    Serial.println("SD CARD "+TEST+FINISHED); 
    showMessageOnLcd(1,17,"SDCard "+TEST,1,45,FINISHED); 
    delay(delay_val);   
  }    
       
void esp8266_setup()
    { 
       // Serial.begin(115200);   // the baud rate here needs to match the esp-link config
      //Serial.println("EL-Client starting!");
      // Sync-up with esp-link, this is required at the start of any sketch and initializes the
      // callbacks to the wifi status change callback. The callback gets called with the initial
      // status right after Sync() below completes.
      esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
      bool ok;
      do 
       {
         ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
         if (!ok) Serial.println("EL-Client sync failed!"); //showMessageOnLcd(20,15, "EL-Client",1,45, "sync failed!");
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
      int err = rest.begin("idiotware.herokuapp.com");
      if(err != 0) 
        {
          Serial.print("REST begin failed: ");
          Serial.println(err);
          while(1) ;
        }
      Serial.println("EL-REST ready");
    }   
void esp8266_TEST() 
    { showMessageOnLcd(20,15, "ESP8266",5,45, TEST+STARTED);
      delay(delay_val); 
      esp8266_setup();
      
      sprintf(buff, "/test");
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
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             showMessageOnLcd(20,15, "ARDUINO:",1,45, "GET failed:");
             Serial.println(code);
            }
          delay(delay_val);
        }
       showMessageOnLcd(20,15, "ESP8266",1,45, TEST+FINISHED);
       delay(delay_val); 
    }  
 

                     
