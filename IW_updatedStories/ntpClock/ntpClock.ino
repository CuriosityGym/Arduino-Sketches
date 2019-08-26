/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  Blynk can provide your device with time data, like an RTC.
  Please note that the accuracy of this method is up to several seconds.

  App project setup:
    RTC widget (no pin required)
    Value Display widget on V1
    Value Display widget on V2

  WARNING :
  For this example you'll need Time keeping library:
    https://github.com/PaulStoffregen/Time

  This code is based on an example from the Time library:
    https://github.com/PaulStoffregen/Time/blob/master/examples/TimeSerial/TimeSerial.ino
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "48689e38ef4943539f450378b74c17bf";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DT_LAB";
char pass[] = "fthu@050318";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial

// or Software Serial on Uno, Nano...
//#include <SoftwareSerial.h>
//SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

WidgetRTC rtc;

const char *const string_table[] PROGMEM = {"Jan", "Feb", "Mar", "Apr", "May", "June",
                                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
                                           };
String thisMonth = "";
String thisTime = "";
String thisDay="";
String AMorPM = "";
int clockCentreX = 32; // used to fix the centre the analog clock
int clockCentreY = 32; // used to fix the centre the analog clock
#define tempPin A0
int date, hours,minutes,seconds;
//int hourOffset;
//int minuteOffset;

// Digital clock display of the time
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  //Serial.print("Current time: ");
  //Serial.print(currentTime);
  //Serial.print(" ");
  //Serial.print(currentDate);
  //Serial.println();
  hours=hour();
  if(hours>12)
    {
     hours = hours%12;
     AMorPM = "pm";
    }
  else
    {
      AMorPM = "am";  
    }
  minutes=minute();
  seconds=second();

   thisDay = String(day());

   int currentMonth = month();

   switch(currentMonth)
    {
      case 1: thisMonth = (char *)pgm_read_word(&(string_table[0]));break;
      case 2: thisMonth = (char *)pgm_read_word(&(string_table[1]));break;
      case 3: thisMonth = (char *)pgm_read_word(&(string_table[2]));break;
      case 4: thisMonth = (char *)pgm_read_word(&(string_table[3]));break;
      case 5: thisMonth = (char *)pgm_read_word(&(string_table[4]));break;
      case 6: thisMonth = (char *)pgm_read_word(&(string_table[5]));break;
      case 7: thisMonth = (char *)pgm_read_word(&(string_table[6]));break;
      case 8: thisMonth = (char *)pgm_read_word(&(string_table[7]));break;
      case 9: thisMonth = (char *)pgm_read_word(&(string_table[8]));break;
      case 10: thisMonth = (char *)pgm_read_word(&(string_table[9]));break;
      case 11: thisMonth = (char *)pgm_read_word(&(string_table[10]));break;
      case 12: thisMonth = (char *)pgm_read_word(&(string_table[11]));break;
    } 
  // Send time to the App
  //Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  //Blynk.virtualWrite(V2, currentDate);
 
}

void draw(void)
    {
      // graphic commands to redraw the complete screen should be placed here  
      //u8g.setFont(u8g_font_profont15);
      //u8g.setFont(u8g_font_timB14); 
    
      // display time in digital format
      thisTime="";
      thisTime=String(hours) + ":";
      if (minutes < 10)
         { 
           thisTime=thisTime + "0"; // add leading zero if required
         } 
      thisTime=thisTime + String(minutes) +  AMorPM ;
    
      const char* newTime = (const char*) thisTime.c_str();
      u8g.drawStr(65,15, newTime); 
     
      //dispaly date
      String newDate = thisDay + thisMonth;
      const char* today = (const char*) newDate.c_str();
      u8g.drawStr(70,35, today);
     // const char* amORpm = (const char*)AMorPM.c_str();
     //  u8g.drawStr(100,35, amORpm);
      // Now draw the clock face
      u8g.drawCircle(clockCentreX, clockCentreY, 30); // main outer circle
      u8g.drawCircle(clockCentreX, clockCentreY, 31);
      u8g.drawCircle(clockCentreX, clockCentreY, 2);  // small inner circle
      u8g.drawCircle(clockCentreX, clockCentreY, 1);
  
     //hour ticks
     for( int z=0; z < 360;z= z + 30 )
        {
          //Begin at 0° and stop at 360°
          float angle = z ;
          angle=(angle/57.29577951) ; //Convert degrees to radians
          int x2=(clockCentreX+(sin(angle)*26));
          int y2=(clockCentreY-(cos(angle)*26));
          int x3=(clockCentreX+(sin(angle)*(26-5)));
          int y3=(clockCentreY-(cos(angle)*(26-5)));
          u8g.drawLine(x2,y2,x3,y3);
        }
     // display second hand
     float angle = seconds*6 ;
     angle=(angle/57.29577951) ; //Convert degrees to radians  
     int x3=(clockCentreX+(sin(angle)*(22)));
     int y3=(clockCentreY-(cos(angle)*(22)));
     u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  
     // display minute hand
     angle = minutes * 6 ;
     angle=(angle/57.29577951) ; //Convert degrees to radians  
     x3=(clockCentreX+(sin(angle)*(22-4)));
     y3=(clockCentreY-(cos(angle)*(22-4)));
     u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  
     // display hour hand
     angle = hours * 30; // + int((initialMinutes / 12) * 6 )   ;
     angle=(angle/57.29577951) ; //Convert degrees to radians  
     x3=(clockCentreX+(sin(angle)*(22-9)));
     y3=(clockCentreY-(cos(angle)*(22-9)));
     u8g.drawLine(clockCentreX,clockCentreY,x3,y3);

     int reading1 = analogRead(tempPin); //delay(10);
     int reading4 = analogRead(tempPin); //delay(10);
     int tempInCelcius = round(( 5.0 * reading4 * 100.0) / 1024.0);    
     String thisTemp1 = String(tempInCelcius) + String(char(176)) +"C";
     const char* thisTemp = (const char*) thisTemp1.c_str();
     u8g.drawStr(70,62,thisTemp); 
   
   }

void setup()
{
  // Debug console
  Serial.begin(115200);
  u8g.setFont(u8g_font_timB14);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Begin synchronizing time
  rtc.begin();

  // Other Time library functions can be used, like:
  //   timeStatus(), setSyncInterval(interval)...
  // Read more: http://www.pjrc.com/teensy/td_libs_Time.html

  // Display digital clock every 10 seconds
  timer.setInterval(1000L, clockDisplay);
}

void loop()
{
  Blynk.run();
  timer.run();
  u8g.firstPage();  
      do 
        {
          draw();
} while( u8g.nextPage() );
}

