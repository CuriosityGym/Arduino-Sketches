/*
 
 
*/
#include <Adafruit_NeoPixel.h>
#include <CGShield.h>
#include <Wire.h>         // Require for I2C communication
CGShield fs;             // Instanciate CGShield instance

#include <ELClient.h>
#include <ELClientRest.h>
char buff[128];

// Initialize a connection to esp-link using the normal hardware serial port both for
// SLIP and for debug messages.
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
    {
      Serial.begin(9600);   // the baud rate here needs to match the esp-link config
      color(255,255,255);
      Serial.println("EL-Client starting!");

      // Sync-up with esp-link, this is required at the start of any sketch and initializes the
      // callbacks to the wifi status change callback. The callback gets called with the initial
      // status right after Sync() below completes.
      esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
      bool ok;
      do 
       {
         ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
         if (!ok) Serial.println("EL-Client sync failed!");
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

      // Set up the REST client to talk to www.maker.ifttt.com, this doesn't connect to that server,
      // it just sets-up stuff on the esp-link side
      int err = rest.begin("maker.ifttt.com");
      if(err != 0) 
        {
          Serial.print("REST begin failed: ");
          Serial.println(err);
          while(1) ;
        }
      Serial.println("EL-REST ready");
      
    }

void loop() 
    {
     new_letter();
    }
 
// this function detects the new letter
int new_letter()
   { 
    
      int light_value1 = analogRead(A3); delay(10);
      int light_value = analogRead(A3);delay(10);
      Serial.println(light_value);
      if(light_value >= 510)
        { 
          sprintf(buff, "/trigger/new_letter/with/key/YOUR_API_KEY");// replace with your channel's MAKER API key
          logToMaker();  //Log to Maker using commands under void LogToMaker()
          // print to the serial port too:              
          Serial.print("New Letter!!");
                   
        }
        delay(50);   
   } 
 
    
void logToMaker()
    {
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {  Serial.println("wifi connected!!");
          // Request  from the previously set-up server
          rest.get((const char*)buff);

          char response[266];
          uint16_t code = rest.waitResponse(response, 266);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println("ARDUINO: GET successful:");
             Serial.println(response);
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          
        }
        
    }   
