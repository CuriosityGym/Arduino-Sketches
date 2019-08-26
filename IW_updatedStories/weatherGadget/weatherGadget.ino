/*
              Weather Gadget
   In this example we are using ESP8266 wifi module and OLED display
 to display weather conditinos of particular city from WeatherDataApi.
              
 WeatherDataApi: Heroku Deployment Application that reduces data 
                 downloaded for OpenWeatherMap Api
  
 To get data from OpenWeatherMap Api, you need to create openweathermap 
 account and then generate API key and use that API Key in this code.                
 In this example we have used ELClient.h library to get data from WeatherDataApi. 
 Here we are sending request for location,current temprature,humnidity and 
 weather descrition. This code allows you to detect your location automatically
 by IP address or to get weather conditions of particular city just change  the 
 city ID in the following code and change autoDetectLocation to false. OLED is 
 used to display this information.
 We are sending request to "idiotware.herokuapp.com" after every one hour to get
 weather information.To change this interval, change value of refreshRate. 
               
*/


#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include "U8glib.h"
#include "icons.h"    // Bitmap weather icons 

#define autoDetectLocation false
//#define refreshRate 15   //send request after ever 60 minutes(one hour) 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
#define tempPin A0  // LM35 is connected to A0 pin

char auth[] = "48689e38ef4943539f450378b74c17bf";
//char buff[90];
//variables to store recieved data
float temperature;
int humidity;
String location = "";
String weatherDescription = "";
int tempInC;
//boolean firstRequest = true;

int tempInCelcius = 0;
//char buff[90];
//String sendData = "";
unsigned long  elapsedTime;
//unsigned long time=0;
//unsigned long samplingTime = 180;   //this variable is interval(in Seconds) at which you want to log the data to thingspeak.
unsigned long duration = 60;     //this variable is duration(in Minutes) which is the total time for which you want to log data.
unsigned long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
int interval1 = 5000;           // interval at which to blink (milliseconds)
int interval2 = 10000;
int interval3 = 15000;
boolean frame1 = true;
boolean frame2 = true;
boolean frame3 = true;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DT_LAB";
char pass[] = "fthu@050318";
// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial

// or Software Serial on Uno, Nano...
//#include <SoftwareSerial.h>
//SoftwareSerial debugSerial(10, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

const char *const string_table[] PROGMEM = {"Clear", "Clouds", "Scattered clouds",
                                            "Broken clouds", "Shower rain", "Rain", "Thunderstorm",
                                            "Snow", "Mist"
                                            //"http://idiotware.herokuapp.com/getCityCountryByIP",
                                            //"http://idiotware.herokuapp.com/weatherDescription?appid=15373f8c0b06b6e66e6372db065c4e46"                                            
};
void setup() 
    {
      Serial.begin(115200);   // the baud rate here needs to match the esp-link config
       u8g.setFont(u8g_font_timB12);
      u8g.firstPage();
      do 
        {  
         //u8g.setFont(u8g_font_timB14);  
         u8g.drawXBMP( 15, 15, cg_logo_width, cg_logo_height,cg_logo_bits);
         u8g.drawStr(45,27,"curiosity"); u8g.drawStr(45,40,"gym"); 
        } while( u8g.nextPage() );
        //delay(5000);
      //u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
       // Set console baud rate
      //debugSerial.begin(115200);
      delay(10);
      // Set ESP8266 baud rate
      EspSerial.begin(ESP8266_BAUD);
      delay(10);

      Blynk.begin(auth, wifi, ssid, pass);
      
    
      timer.setInterval(20000L, get_location);
      timer.setInterval(23000L, get_weatherDescription);
      timer.setInterval(26000L, get_Humidity);
      timer.setInterval(29000L, get_Temperature);
      duration = 2800 * 60;
    }
    
BLYNK_WRITE(V0)
{
  location = param.asStr();
  //Blynk.virtualWrite(V4,param.asStr());
}    
void get_location()
{
  // You can perform HTTPS requests even if your hardware alone can't handle SSL
  // Blynk  can also fetch much bigger messages,
  // if hardware has enough RAM (set BLYNK_MAX_READBYTES to 4096)
  //Blynk.virtualWrite(V0, "https://api.sunrise-sunset.org/json?lat=50.4495484&lng=30.5253873&date=2016-10-01");
  ///if(autoDetectLocation)
      //  {
           //sprintf(buff, "/getCityCountryByIP");
           Blynk.virtualWrite(V0, (char *)pgm_read_word(&(string_table[9])));//"http://idiotware.herokuapp.com/getCityCountryByIP");
        //}
      //else
       // {*/
           //sprintf(buff, "/getCityCountry?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           //Blynk.virtualWrite(V0, (char *)pgm_read_word(&(string_table[0])));//"http://idiotware.herokuapp.com/getCityCountry?id=1275339&appid=15373f8c0b06b6e66e6372db065c4e46");
         //}    
}

BLYNK_WRITE(V1)
{
  weatherDescription = param.asStr();
  //Blynk.virtualWrite(V2,param.asStr());
}    
void get_weatherDescription()
{
  // You can perform HTTPS requests even if your hardware alone can't handle SSL
  // Blynk  can also fetch much bigger messages,
  // if hardware has enough RAM (set BLYNK_MAX_READBYTES to 4096)
  //Blynk.virtualWrite(V0, "https://api.sunrise-sunset.org/json?lat=50.4495484&lng=30.5253873&date=2016-10-01");
 // if(autoDetectLocation)
      //  {
           //sprintf(buff, "/getCityCountryByIP");
          Blynk.virtualWrite(V1, (char *)pgm_read_word(&(string_table[9])));//"http://idiotware.herokuapp.com/weatherDescription?appid=15373f8c0b06b6e66e6372db065c4e46");
       // }
     // else
      //  {
           //sprintf(buff, "/getCityCountry?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           //Blynk.virtualWrite(V1, "http://idiotware.herokuapp.com/weatherDescription?id=1275339&appid=15373f8c0b06b6e66e6372db065c4e46");
        //}    
}

BLYNK_WRITE(V2)
{
   String hum = param.asStr();  // convert recieved string to integer
   humidity = hum.toInt();
 /* if(hum >=101 && hum<200)
    { hum = 100;
      humidity = hum;
    } 
  else if(hum >199) 
    {
     hum = hum/10;
     humidity = hum; 
    }  
  else
    {
     humidity = hum; 
    }*/
 //Blynk.virtualWrite(V6,humidity);    
}    
void get_Humidity()
{
  // You can perform HTTPS requests even if your hardware alone can't handle SSL
  // Blynk  can also fetch much bigger messages,
  // if hardware has enough RAM (set BLYNK_MAX_READBYTES to 4096)
  //Blynk.virtualWrite(V0, "https://api.sunrise-sunset.org/json?lat=50.4495484&lng=30.5253873&date=2016-10-01");
  //if(autoDetectLocation)
      //  {
           //sprintf(buff, "/getCityCountryByIP");
           Blynk.virtualWrite(V2, (char *)pgm_read_word(&(string_table[9])));//"http://idiotware.herokuapp.com/humidity?id=1275339&appid=15373f8c0b06b6e66e6372db065c4e46");

      //  }
     // else
        //{
           //sprintf(buff, "/getCityCountry?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           //Blynk.virtualWrite(V2, (char *)pgm_read_word(&(string_table[0])));//"http://idiotware.herokuapp.com/humidity?appid=15373f8c0b06b6e66e6372db065c4e46");
       // }    
}

BLYNK_WRITE(V3)
{
  String temperature = param.asStr();  // convert recieved string to integer
  float temp = temperature.toFloat();
  tempInC = (temp - 273.15);
  //Blynk.virtualWrite(V5,tempInC);
}    
void get_Temperature()
{
  // You can perform HTTPS requests even if your hardware alone can't handle SSL
  // Blynk  can also fetch much bigger messages,
  // if hardware has enough RAM (set BLYNK_MAX_READBYTES to 4096)
  //Blynk.virtualWrite(V0, "https://api.sunrise-sunset.org/json?lat=50.4495484&lng=30.5253873&date=2016-10-01");
  //if(autoDetectLocation)
      //  {
           //sprintf(buff, "/getCityCountryByIP");
           Blynk.virtualWrite(V3,(char *)pgm_read_word(&(string_table[9])));// "http://idiotware.herokuapp.com/weatherDescription?appid=15373f8c0b06b6e66e6372db065c4e466");

       // }
      //else
       // {
           //sprintf(buff, "/getCityCountry?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           //Blynk.virtualWrite(V3,(char *)pgm_read_word(&(string_table[0])));// "http://idiotware.herokuapp.com/temperature?id=1275339&appid=15373f8c0b06b6e66e6372db065c4e46");
        //}    
}
void loop() 
    { 
      Blynk.run();
      timer.run();
      //elapsedTime = millis()/1000;   // this variable will keep track of elapsed time
      //while(((millis()/1000)-elapsedTime) < 1);    // this loop will do nothing until a second has passed 
      //time++;                                       //increment time after each second.

      //weatherData();
      //dataSamples();
  
      frameScroll();
  
    }      

void displayIcon() 
    {
      //"clear-day, clear-night, rain, snow, sleet, wind, fog, cloudy, partly-cloudy-day, or partly-cloudy-night"
      if (weatherDescription == (char *)pgm_read_word(&(string_table[0]))) 
         {
          drawFrame1(clear_sky);
         } 
      else if (weatherDescription == (char *)pgm_read_word(&(string_table[1]))) 
         {
          drawFrame1(few_clouds);
         } 
      else if (weatherDescription == (char *)pgm_read_word(&(string_table[2]))) 
         {
          drawFrame1(scattered_clouds);
         }
      else if (weatherDescription == (char *)pgm_read_word(&(string_table[3]))) 
         {
          drawFrame1(broken_clouds);
         } 
      else if (weatherDescription == (char *)pgm_read_word(&(string_table[4]))) 
         {
          drawFrame1(shower_rain);
         }
      else if (weatherDescription == (char *)pgm_read_word(&(string_table[5]))) 
         {
          drawFrame1(rain);
         } 
      else if (weatherDescription == (char *)pgm_read_word(&(string_table[6]))) 
         {
          drawFrame1(thunderstorm);
         } 
      else if (weatherDescription == (char *)pgm_read_word(&(string_table[7]))) 
         {
          drawFrame1(snow);
         } 
      else if (weatherDescription == (char *)pgm_read_word(&(string_table[8]))) 
         {
          drawFrame1(mist);
         } 
      else 
         {
          drawFrame1(no_icon);
         } 
    } 

//print weather icon and weather description on OLED
void drawFrame1(const uint8_t *bitmap) 
    { 
      
      
       u8g.setPrintPos( 0, 11);
       u8g.print(location);
       u8g.drawXBMP(  40, 12, icon_width, icon_height, bitmap);
       u8g.setPrintPos(  10, 61);
       u8g.print(weatherDescription);
     
    }
    
//print thermometer icon and tmeperature and humidity values on OLED
void drawFrame2() 
    {  
      
       u8g.setPrintPos(0, 11);
       u8g.print(location);
       u8g.setPrintPos( 59 , 31);
       u8g.print(tempInC);
       u8g.drawXBMP( 75 , 15, icon_width, icon_height, temperature_icon);
       u8g.drawStr(  0, 45, "Hum:");
       u8g.setPrintPos(  0, 60);
       u8g.print(humidity);
       if(humidity == 100)
         {
           u8g.drawXBMP( 25, 50, percentage_sign_WIDTH,percentage_sign_HEIGHT, percentage_sign);
         }
       else
         {
           u8g.drawXBMP( 20 , 50, percentage_sign_WIDTH,percentage_sign_HEIGHT, percentage_sign);
         }  
      
    }

void drawFrame3() 
    {
     int reading1 = analogRead(tempPin);// delay(10);
     int reading2 = analogRead(tempPin);// delay(10);
     int reading3 = analogRead(tempPin);// delay(10);
     int reading4 = analogRead(tempPin);// delay(10);
      
     int averageReading = ((reading2 +  reading3 + reading4 )/ 3); 
     tempInCelcius = round(( 5.0 * averageReading * 100.0) / 1024.0);
     // u8g.setFont(u8g_font_timB14);
      u8g.drawStr(0,15, "Room Temp: ");
    //  u8g.setPrintPos( 0, 11);
    //   u8g.print("Room Temp:");
     //u8g.setFont(u8g_font_timB14);
     
      
       u8g.setPrintPos(45, 40); 
      u8g.print(tempInCelcius );
      u8g.setPrintPos(67, 40);
      u8g.print(char(176));
      u8g.drawStr( 75, 40, "C");
     //  u8g.setFont(u8g_font_timB12);
      
     // u8g.drawXBMP( 55 , 25, icon_width, icon_height, temperature_icon);
          
    }

   
// function to scroll frames on OLED after 5 seconds  
void frameScroll()
    { unsigned long currentMillis = millis();
      if(currentMillis - previousMillis > interval1 && frame1 == true)
        {
          u8g.firstPage();
          do 
            {  
              displayIcon();
              frame1 = false;
            } while( u8g.nextPage() );}
 
      if(currentMillis - previousMillis > interval2 && frame2 == true) 
        {
          u8g.firstPage();
          do 
            {  
              drawFrame2();
              frame2 = false;
            } while( u8g.nextPage() );
        
            }
      
      if(currentMillis - previousMillis > interval3 && frame3==true) 
        {
          u8g.firstPage();
          do 
            {  
              drawFrame3();
              frame3 = false;
            } while( u8g.nextPage() );
     
       previousMillis = currentMillis;
       frame1 = true;
       frame2 = true;
       frame3 = true;
     }
   
   }
   
