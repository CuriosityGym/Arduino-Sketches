/*
  ESP8266 based temperature logging to Thinspeak
 
 This example shows how to log data from Arduino to Thingspeak using ESP8266 wifi module.
 ThingSpeak is an open source Internet of Things (IoT) application and API to store and 
 retrieve data from things using the HTTP protocol over the Internet or via a Local Area Network.
 ThingSpeak enables the creation of sensor logging applications, location tracking applications,
 and a social network of things with status updates.
 
 To log the data to thingspeak, you need to create thingspeak account and then create channel and 
 use that channel's API Key in this code.
 
 In this example we have used ELClient.h library to log data to thingspeak. Temperature values from
 LM35 temperature sensor are logged to thingspeak every 16 seconds for 30 minutes. This interval rate 
 and duration is configurable. thingspeak needs 15 sec delay between updates.
 
*/

#include <ELClient.h>
#include <ELClientRest.h>
const byte tempPin =A0;  // LM35 is connected to A0 pin
float tempInCelcius;
char buff[64];
String sendData = "";
unsigned long time=0;
int samplingTime = 300;  //this variable is interval(in Seconds) at which you want to log the data to SD card.
unsigned int duration = 3000;     //this variable is duration(in Minutes) which is the total time for which you want to log data.

// replace with your channel's thingspeak API key
String API_KEY = "BNOJ3CIAYB2DJPDT";
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
      Serial.begin(115200);   // the baud rate here needs to match the esp-link config
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

      // Set up the REST client to talk to www.timeapi.org, this doesn't connect to that server,
      // it just sets-up stuff on the esp-link side
      int err = rest.begin("api.thingspeak.com");
      if(err != 0) 
        {
          Serial.print("REST begin failed: ");
          Serial.println(err);
          while(1) ;
        }
      Serial.println("EL-REST ready");
      duration *= 60;   //convert durartion in minutes to seconds
    }

void loop() 
    {
     dataSamples();
    }
 
 
int dataSamples()
   { 
     // here we are logging data at interval of 16 seconds for 30 mintutes, i.e, 112 samples.
     // if you want to log data for 2 hours then simply multiply 2 by 60 which will give 
     // you value of 120 minutes then change the varible duration to 120. 

      unsigned long  elapsedTime = millis()/1000;   // this variable will keep track of elapsed time
      while(((millis()/1000)-elapsedTime) < 1);    // this loop will do nothing until a second has passed 
      time++;                                       //increment time after each second.
 
      if((duration >= time) && (time % samplingTime == 0))
        { 
          tempInCelcius = ( 5.0 * analogRead(tempPin) * 100.0) / 1024.0;
          int light_value = analogRead(A3);
          char str_light[6]; 
          char str_temp[6];
          dtostrf(tempInCelcius, 4, 2, str_temp);
          dtostrf(light_value, 4, 2, str_light);
          sprintf(buff, "/update?api_key=%s&field1=%s&field2=%s",API_KEY.c_str(), str_temp,str_light);
          // uncomment following line to get temperature values in Farehniet
          //tempInFarenheit = ((tempC*9)/5) + 32;            //convert celcius to farenheit
          // print to the serial port too:              
          Serial.print("Temperature: ");
          Serial.print(tempInCelcius);
          Serial.print(char(176)); 
          Serial.println("C"); 
          logToThingspeak();  //Log to thingspeak using commands under void LogToThingspeak()      
        }
           
   } 
 
    
void logToThingspeak()
    {
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
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
    
   
   
