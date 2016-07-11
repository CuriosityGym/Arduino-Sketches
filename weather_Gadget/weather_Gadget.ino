#include "U8glib.h"
#include <ELClient.h>
#include <ELClientRest.h>
char buff[512];
float temperature;
String humidity = "";
String location = "";
String weatherDescription = "";
float tempInCelsius;
// replace with your channel's thingspeak API key
String API_KEY = "15373f8c0b06b6e66e6372db065c4e46";
String CityID = "1275339"; //Mumbai, India

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);
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
      Serial.println("EL-Client starting!");
      //u8g.setFont(u8g_font_timB12);
      u8g.setColorIndex(1);
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
      int err = rest.begin("idiotware.herokuapp.com");
      if(err != 0) 
        {
          Serial.print("REST begin failed: ");
          Serial.println(err);
          while(1) ;
        }
      Serial.println("EL-REST ready");
     
     
    }

void loop() 
    { get_location();
      get_Temperature();
      get_Humidity();
      get_weatherDescription();
      showMessageOnLcd();
      delay(5000);
    }
 

void get_location()
    { 
      sprintf(buff, "/getCityCountry");
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
             Serial.print("Response: ");
             Serial.println(response);
             location = response;
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          delay(1000);
        }
        
    }   
    
void get_Temperature()
    { 
      sprintf(buff, "/temperature?id=1275339&appid=15373f8c0b06b6e66e6372db065c4e46");
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[3];
          uint16_t code = rest.waitResponse(response, 20);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println("ARDUINO: GET successful:");
             Serial.print("Response: ");
             temperature = atoi(response);
             Serial.println(response);
             tempInCelsius = (temperature - 273.15);
             Serial.println(tempInCelsius);
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          delay(1000);
        }
        
    }   
    
     
void get_Humidity()
    { 
      sprintf(buff, "/humidity?id=1275339&appid=15373f8c0b06b6e66e6372db065c4e46");
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[3];
          uint16_t code = rest.waitResponse(response, 3);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println("ARDUINO: GET successful:");
             Serial.print("Response: ");
             Serial.println(response);
             humidity = response;
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          delay(1000);
        }
        
    }   
     

      
void get_weatherDescription()
    { 
      sprintf(buff, "/weatherDescription?id=1275339&appid=15373f8c0b06b6e66e6372db065c4e46");
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
             Serial.print("Response: ");
             Serial.println(response);
             weatherDescription = response;
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          delay(1000);
        }
        
    }   
    
void showMessageOnLcd()
    {u8g.firstPage();
      do { u8g.setFont(u8g_font_timB10);
           u8g.setPrintPos(5, 15);
           u8g.print(location);
           u8g.setFont(u8g_font_timB12);
           u8g.setPrintPos(70, 30);
           u8g.print(tempInCelsius);
           u8g.drawStr( 110, 30, "C");
           u8g.drawStr( 65, 45, "Hum: ");
           u8g.setPrintPos(105, 45);
           u8g.print(humidity);
           u8g.setPrintPos(80, 60);
           u8g.print(weatherDescription);  
            
         } while( u8g.nextPage() );
    }    
    
