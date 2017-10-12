#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>
 
#include <TimeLib.h>
#include <NeoPixelBus.h>

const uint16_t PixelCount = 28; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(255,0, 0);
RgbColor green(0,255,0);
RgbColor blue(0,0, 255);
RgbColor yellow(255,255, 0);
RgbColor black(0,0, 0);
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "9b8a99fed224476e9121ffbb1e273414";
BlynkTimer timer;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CuriosityGym-BCK";
char pass[] = "#3Twinkle3#";

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
unsigned long interval = 120000;
int previousPinValue = 0;
int pinValue;
boolean set = true;
boolean check = true;
int buzzer = 4;
boolean buzzerHigh = false; 
boolean sound = false;
int i = 0;
int buzzerCount = 25;
int buzzerInterval = 500;
// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V1)
{
   pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("Pin Value: ");
  Serial.println(pinValue);
 
  if(previousPinValue != pinValue)
    {
      set = true;
      check = true;
      buzzerInterval = 500;
        i=0;
        buzzerHigh = true;
        sound = true;
    }
  if((pinValue < 7) && set == true)
    {
       
        for(int i=0; i<PixelCount; i++)
           { 
             strip.SetPixelColor(i,RgbColor(black));
             if(i == pinValue) strip.SetPixelColor(pinValue, RgbColor(yellow));
             Serial.print("Pin Value: ");
             Serial.println(pinValue);
           }
        strip.Show();
        previousPinValue = pinValue;
        set = false;
        
    }
  if((pinValue > 6) && (pinValue < 14) && set == true)
    {  
        int led = map(pinValue, 7,13,13,7);
        for(int i=0; i<PixelCount; i++)
           { 
             strip.SetPixelColor(i,RgbColor(black));
             if(i == led) strip.SetPixelColor(led, RgbColor(green));
             Serial.print("Pin Value: ");
             Serial.println(led);
           }
        strip.Show();
        previousPinValue = pinValue;
        set = false;
    }
  if((pinValue >13) && (pinValue < 21) && (set == true))
    {
        for(int i=0; i<PixelCount; i++)
           { 
             strip.SetPixelColor(i,RgbColor(black));
             if(i == pinValue) strip.SetPixelColor(pinValue, RgbColor(red));
             Serial.print("Pin Value: ");
             Serial.println(pinValue);
           }
        strip.Show();
        previousPinValue = pinValue;
        set = false;
    }
   if((pinValue >20) && (pinValue < 28) && (set == true))
    {
        int led1 = map(pinValue,21,27,27,21); 
        Serial.print("led1: ");
        Serial.println(led1);
        for(int i=0; i<PixelCount; i++)
           { 
             strip.SetPixelColor(i,RgbColor(black));
             if(i == led1) strip.SetPixelColor(led1, RgbColor(blue));
             Serial.print("Pin Value: ");
             Serial.println(led1);
           }
        strip.Show();
        previousPinValue = pinValue;
        set = false;
        
    } 
 
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  WiFiManager wifiManager;
   // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();
   // wifiManager.resetSettings();
     wifiManager.autoConnect("Pill_Reminder");
     Serial.println("connected...yeey :)");
     Serial.println( WiFi.SSID().c_str());
     Serial.println(WiFi.psk().c_str());
    //String ssid1 = WiFi.SSID().c_str();
    wifiManager.setConfigPortalTimeout(180);  // after 3 minutes try to autoconnect again
  //Blynk.begin(auth, ssid, pass);
  Blynk.config(auth);
  strip.SetPixelColor(27,RgbColor(green));
  strip.Show();
  delay(1000);
  strip.SetPixelColor(27,RgbColor(black));
  strip.Show();
 // Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
  //timer.setInterval(1000L,  ledOff);
}


void loop()
{
  Blynk.run();
   if(previousPinValue == pinValue)
    { 
      currentMillis = millis();
      if(check == true)
        {
          previousMillis = currentMillis;
          check = false;
        }

     if(sound == true)
       {    
         if((currentMillis - previousMillis1 < buzzerInterval) && (i< buzzerCount) && (buzzerHigh == true))
            {
             digitalWrite(buzzer, HIGH);
            }
         if((currentMillis - previousMillis1 > buzzerInterval)&& (buzzerHigh == true))
            {
              i++;
              buzzerHigh = false;
              previousMillis1 = currentMillis;      
            }
     
         if((currentMillis - previousMillis1 <  buzzerInterval) && (i < buzzerCount) && (buzzerHigh == false))
            {
              digitalWrite(buzzer, LOW);
            } 
         if((currentMillis - previousMillis1 > buzzerInterval)&& (buzzerHigh == false))
            {
              i++;
              buzzerHigh = true;
              previousMillis1 = currentMillis;      
            }
         if(i==20) sound = false;   
       }
      if(currentMillis - previousMillis > interval)
           {
             for(int i=0; i<PixelCount; i++)
           { 
             strip.SetPixelColor(i,RgbColor(black));
           }
        strip.Show();
        digitalWrite(buzzer, LOW);
           }      
    }
}
