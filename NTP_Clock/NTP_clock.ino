// Add libraries
#include <ELClient.h>
#include <ELClientRest.h>
#include "U8glib.h"
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>
#define BUFLEN 64  
// setup u8g object
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C 
//

String monthArray[]= {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
//int  monthIndex[122] ={0,3,6,9,12,15,18,21,24,27,30,33};
String dayArray[]={"Sun","Mon","Tues","Wed","Thurs","Fri","Sat"};
int years;
String thisMonth = "";
String thisTime = "";
int thisDay;
int weekdays = 0;
String dayOftheWeek="";
String AMorPM = "";
int clockCentreX = 32; // used to fix the centre the analog clock
int clockCentreY = 32; // used to fix the centre the analog clock
int tempPin = A0; 
int date, hours,minutes,seconds;
int hourOffset;
int minuteOffset;
unsigned long currentMillis;
unsigned long previousMillis;
unsigned long previousMillis1;
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
               Serial.println("WIFI CONNECTED");
               wifiConnected = true;
             } 
           else 
             {
               Serial.print("WIFI NOT READY: ");
               Serial.println(status);
               wifiConnected = false;
             }
         }
    }

void draw(void)
    {
      // graphic commands to redraw the complete screen should be placed here  
      //u8g.setFont(u8g_font_profont15);
      u8g.setFont(u8g_font_timB14); 
    
      // display time in digital format
      thisTime="";
      thisTime=String(hours) + ":";
      if (minutes < 10)
         { 
           thisTime=thisTime + "0"; // add leading zero if required
         } 
      thisTime=thisTime + String(minutes) +  AMorPM ;
    
      const char* newTime = (const char*) thisTime.c_str();
      u8g.drawStr(60,15, newTime); 
     
      //dispaly date
      String blank = " ";
      String newDate = thisDay+ blank + thisMonth;
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
/*
     int reading1 = analogRead(tempPin); //delay(10);
     int reading4 = analogRead(tempPin); //delay(10);
     int tempInCelcius = round(( 5.0 * reading4 * 100.0) / 1024.0);    
     String thisTemp1 = String(tempInCelcius) + String(char(176)) +"C";
     const char* thisTemp = (const char*) thisTemp1.c_str();
     u8g.drawStr(70,62,thisTemp); 
   */
   const char* thisDay = (const char*) dayOftheWeek.c_str();
   u8g.drawStr(70,55,thisDay);
   }

void setup(void)
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
      if ((packet=esp.WaitReturn()) != NULL) 
         {
           Serial.print("Wifi status: ");
           Serial.println(packet->value);
         }

     // getTimeZone();
       getTime();
  
    }



void loop(void) 
    { 

      currentMillis = millis();
      if((currentMillis - previousMillis) > 3600000)
        {
          getTime();
          previousMillis = currentMillis;
        }
       if((currentMillis - previousMillis1) > 920)
        {
          seconds= seconds+1;
          previousMillis1 = currentMillis;
          if(seconds > 60)
             {
              minutes = minutes+1;
              seconds = 0;
              
             }  
          if(minutes == 60)
             {
              minutes = 0;
              hours = hours+1;
              
             }
          if(hours == 24)
             {
              minutes = 0;
              hours = 0;
              seconds=0;
              
             }                 
        } 
      // picture loop
      u8g.firstPage();  
      do 
        {
          draw();
        } while( u8g.nextPage() );
    }

void getTimeZone()
    {
      //idiotware.herokuapp.com/getTimeZoneOffset?appid=AIzaSyARAKu23k_1vltEKesq6REtMSIflQ3c3jo
      
       // Set up the REST client to talk to www.timeapi.org, this doesn't connect to that server,
      // it just sets-up stuff on the esp-link side
      int err = rest.begin("idiotware.herokuapp.com");
      if (err != 0)
         {
           Serial.print("REST begin failed: ");
           Serial.println(err);
           while(1) ;
         }
      Serial.println("EL-REST ready");
       // process any callbacks coming from esp_link
      esp.Process();

      // if we're connected make an HTTP request
      if(wifiConnected)
        {
          // Request /utc/now from the previously set-up server
          rest.get("/getTimeZoneOffset?appid=AIzaSyARAKu23k_1vltEKesq6REtMSIflQ3c3jo");

          char response[BUFLEN];
          memset(response, 0, BUFLEN);
          uint16_t code = rest.waitResponse(response, BUFLEN);
          if(code == HTTP_STATUS_OK){
          Serial.println("ARDUINO: GET successful:");
          Serial.println(response);
          signed long offsetTime = atoi(response);
          hourOffset = offsetTime/3600;
          minuteOffset = (offsetTime%3600)/60;
          Serial.println(hourOffset);Serial.println(minuteOffset);
          
        }
         else
        {
          Serial.print("ARDUINO: GET failed: ");
          Serial.println(code);
        }
        }
    }
void getTime()
    { 
      // Set up the REST client to talk to www.timeapi.org, this doesn't connect to that server,
      // it just sets-up stuff on the esp-link side
      int err = rest.begin("idiotware.herokuapp.com");
      if (err != 0)
         {
           Serial.print("REST begin failed: ");
           Serial.println(err);
           while(1) ;
         }
      Serial.println("EL-REST ready");
      // process any callbacks coming from esp_link
      esp.Process();

      // if we're connected make an HTTP request
      if(wifiConnected)
        {
          // Request /utc/now from the previously set-up server
          rest.get("/getLocalTime?appid=AIzaSyARAKu23k_1vltEKesq6REtMSIflQ3c3jo");

          char response[BUFLEN];
          memset(response, 0, BUFLEN);
          uint16_t code = rest.waitResponse(response, BUFLEN);
          if(code == HTTP_STATUS_OK){
          Serial.println("ARDUINO: GET successful:");
          Serial.println(response);
          String a = response;
          long unsigned int t= a.toInt();
          Serial.println(t);
          //String data(response);
          Serial.println(hour(t));          // returns the hour for the given time t
          Serial.println(minute(t));        // returns the minute for the given time t
          Serial.println(second(t));        // returns the second for the given time t
          Serial.println(day(t));           // the day for the given time t
          Serial.println(weekday(t));       // day of the week for the given time t
          Serial.println(month(t));         // the month for the given time t
          Serial.println(year(t));
          thisDay = day(t);
         // thisDate = atoi(date.c_str());
         // String month = data.substring(5,7);
          int currentMonth = month(t);
          weekdays = weekday(t);
          years= year(t);
          switch(currentMonth)
                {
                  case 1: thisMonth = monthArray[currentMonth-1];break;
                  case 2: thisMonth = monthArray[currentMonth-1];break;
                  case 3: thisMonth = monthArray[currentMonth-1];break;
                  case 4: thisMonth = monthArray[currentMonth-1];break;
                  case 5: thisMonth = monthArray[currentMonth-1];break;
                  case 6: thisMonth = monthArray[currentMonth-1];break;
                  case 7: thisMonth = monthArray[currentMonth-1];break;
                  case 8: thisMonth = monthArray[currentMonth-1];break;
                  case 9: thisMonth = monthArray[currentMonth-1];break;
                  case 10: thisMonth = monthArray[currentMonth-1];break;
                  case 11: thisMonth = monthArray[currentMonth-1];break;
                  case 12: thisMonth = monthArray[currentMonth-1];break;
                }  
             switch(weekdays)
                {
                  case 1: dayOftheWeek = dayArray[weekdays-1];break;
                  case 2: dayOftheWeek = dayArray[weekdays-1];break;
                  case 3: dayOftheWeek = dayArray[weekdays-1];break;
                  case 4: dayOftheWeek = dayArray[weekdays-1];break;
                  case 5: dayOftheWeek = dayArray[weekdays-1];break;
                  case 6: dayOftheWeek = dayArray[weekdays-1];break;
                  case 7: dayOftheWeek = dayArray[weekdays-1];break;
                   }      
         
         // String hour = hour(t);
          hours = hour(t);
         // String minute = data.substring(14,16);
          minutes = minute(t);
          /*int hoursInMinutes = hours*60;
         
           hours = hours + hourOffset-1; 
           if(hours>12)
            {
              hours = hours%12;
            }  
          
          minutes = minutes + minuteOffset;
          if(minutes>=60)
            {
              minutes = minutes%60;
              hours += 1;
             }  */
         // String second = data.substring(17,19);
          seconds = second(t);
              if(hours  < 12 )// minutes >= minuteOffset)
                {
                  AMorPM = "am"; 
                }
              if(hours >= 12)// && minutes >= minuteOffset)
                {
                  AMorPM = "pm";
                }
     
          Serial.println("hours: ");
          Serial.println(hours);
          Serial.println("minutes: ");
          Serial.println(minutes);
          Serial.println("seconds: ");
          Serial.println(seconds);
          
        } 
      else
        {
          Serial.print("ARDUINO: GET failed: ");
          Serial.println(code);
        }
     }
  }
