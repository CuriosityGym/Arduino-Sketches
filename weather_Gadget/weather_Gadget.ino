
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ELClient.h>
#include <ELClientRest.h>
#include "icons.h"
#include <Fonts/FreeMono9pt7b.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
char buff[512];
float temperature;
int humidity;
String location = "";
String weatherDescription="";
int tempInCelsius;
// replace with your channel's thingspeak API key
String API_KEY = "15373f8c0b06b6e66e6372db065c4e46";
String CityID = "1275339"; //Mumbai, India


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
      display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setFont(&FreeMono9pt7b);
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
      displayIcon();
      delay(5000);
      drawFrame2();
      delay(5000);
    }
 

void get_location()
    { 
      sprintf(buff, "/getCityCountry?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[20]="";
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
      sprintf(buff, "/temperature?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[]="";
          uint16_t code = rest.waitResponse(response, 3);
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
      sprintf(buff, "/humidity?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[]="";
          uint16_t code = rest.waitResponse(response, 2);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println("ARDUINO: GET successful:");
             Serial.print("Response: ");
             Serial.println(response);
             int hum = atoi(response);
             if (hum > 100)
                { hum = hum/10;
                  humidity = hum;Serial.print(hum);
                } 
             else 
                {
                  humidity = hum; Serial.print(hum);
                }  
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
      sprintf(buff, "/weatherDescription?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[20]="";
          uint16_t code = rest.waitResponse(response, 20);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println("ARDUINO: GET successful:");
             Serial.print("Response: ");
             Serial.println(response);
             //weather(response);
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
   
void weather( char data[]){
  int EOT = 4;
  char weatherDescription1[20];
for (int i = 0; i < 20; i++){
   Serial.println(data);
   data[i] = weatherDescription1[i];
   if(data[i] == EOT)
   break;
   
   }
   Serial.print("weatherDescription: ");Serial.print(weatherDescription1);
}    
void displayIcon() 
    {
      //"clear-day, clear-night, rain, snow, sleet, wind, fog, cloudy, partly-cloudy-day, or partly-cloudy-night"
      if (weatherDescription == "Clear") 
         {
          drawFrame1(clear_sky);
         } 
      else if (weatherDescription == "Clouds") 
         {
          drawFrame1(few_clouds);
         } 
      else if (weatherDescription == "Scattered clouds") 
         {
          drawFrame1(scattered_clouds);
         }
      else if (weatherDescription == "Broken clouds") 
         {
          drawFrame1(broken_clouds);
         } 
      else if (weatherDescription == "Shower rain") 
         {
          drawFrame1(shower_rain);
         }
      else if (weatherDescription == "Rain") 
         {
          drawFrame1(rain);
         } 
      else if (weatherDescription == "Thunderstorm") 
         {
          drawFrame1(thunderstorm);
         } 
      else if (weatherDescription == "Snow") 
         {
          drawFrame1(snow);
         } 
      else if (weatherDescription == "Mist") 
         {
          drawFrame1(mist);
         } 
      else 
         {
          drawFrame1(no_icon);
         }
  //return cloudy_bits;
}    
void drawFrame1(const uint8_t *bitmap) 
    { 
      display.clearDisplay();
      display.drawBitmap(40, 12, bitmap, 40, 40, 1);
      display.setCursor(1,9);
      display.println(location);
      display.setCursor(10,60);
      display.println(weatherDescription);
      display.display();
    }

void drawFrame2() 
    { display.clearDisplay();
      display.drawBitmap(75, 15, temperature_icon, 40, 40, 1);
      display.setCursor(1,9);
      display.println(location);
      display.setCursor(58,28);
      display.println(tempInCelsius);
      display.setCursor(0,45);
      display.println("Hum: ");
      display.setCursor(0,60);
      display.println(humidity);
      if(humidity == 100)
        {
         display.drawBitmap(35, 50, percentage_sign, 10, 10, 1);
        }
      else
        {
         display.drawBitmap(25, 50, percentage_sign, 10, 10, 1);
        }  
      display.display();
    }
