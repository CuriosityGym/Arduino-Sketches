/*
    Back Door Intrusion Detector
    
    In this example we are using ESP8266 and IdIoTWare Shield and Arduino Board. Magnetic Reed Switch
    is used to detect intrusion.
    Magnetic Reed Switch :
    A magnetic contact switch is basically a reed switch encased in a plastic shell so that
    you can easily apply them in a door, a window or a drawer to detect if the door is open or closed..

    
    This example uses a  Magnetic Reed Switch The electrical circuit is closed when a magnet
    is near the switch (less than 13 mm (0.5’’) away). When the magnet is far away from the switch, the circuit
    is open.
    When it is open, a signal is  sent to  arduino uno that sends email to user using ESP8266.
    Here we are using IFTTT to trigger an event.
    
    IFTTT is a free web-based service that allows users to create chains of simple conditional statements,
    called "recipes", which are triggered based on changes to other web services such as Gmail, Facebook,
    Instagram, and many more.IFTTT is an abbreviation of "If This Then That"
    Create account on IFTTT and create your recipe.
    
    We are using Maker and Gmail channel to trigger an event.
    
    If magnetic reed switch is open(if door is open), Arduino will send POST request to maker channel.  
    if there is any event on Maker channel  then send mail using Gmail
    If Maker Event "motion_detected", then send an email from "abcd@gmail.com" 
    
      
*/
#include <Adafruit_NeoPixel.h>

#include <ELClient.h>
#include <ELClientRest.h>
#include <Adafruit_NeoPixel.h>
#include <idIoTwareShield.h>
#include <Wire.h>         // Require for I2C communication
idIoTwareShield fs;             // Instanciate CGShield instance

char buff[128];
int inputPin = 2;               // choose the input pin (for PIR sensor)
int doorState = LOW;
int inputPin_state = 0;              // variable for reading the pin status
int buzzerPin = A1;
// replace with your channel's thingspeak API key
String API_KEY = "cDlIAZcApEGZiZDeOIXExA";//bEJ04i0uriNNFOfaM9QrAf
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
      pinMode(inputPin, INPUT_PULLUP);     // declare PIR sensor as input
      pinMode(buzzerPin,OUTPUT);     
      Serial.begin(9600);   // the baud rate here needs to match the esp-link config
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
     door_opened();
    }
 
// this function detects the new letter
int door_opened()
   { 
    
      inputPin_state = digitalRead(inputPin);  // read input value
      if (inputPin_state == HIGH) 
         { // check if the input is HIGH 
           //if (doorState == LOW) 
             // {
                sprintf(buff, "/trigger/door_opened/with/key/iYiYhj3KyPFEwyVRuJzEb");
                logToMaker();  //Log to Maker using commands under void LogToMaker()
                // print to the serial port too:              
                Serial.print("door_opened!!");
                for(int i=0; i<20; i++)
                   {
                     beep(250);
                   } 
              //}    
         }
          
   } 
 
//function to send POST request to Maker channel    
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
    
void beep(int delayValue)
     {
       digitalWrite(buzzerPin,HIGH);
       color(255,0,0);
       delay(delayValue);
       digitalWrite(buzzerPin,LOW);
       color(0,0,0);
     }    
