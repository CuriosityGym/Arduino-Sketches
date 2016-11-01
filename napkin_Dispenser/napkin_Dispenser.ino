#include <Adafruit_NeoPixel.h>

#define PIN 7
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
int buzzerPin = A1;

const byte dispensedSensorPin = 3;  // This is our sensor2 pin
const byte moneySensorPin = 4;  // This is our sensor1 pin
int firstSensor = 5;
int secondSensor = 6;
//boolean money = false;
boolean reading = false;
const byte motor = 9;           // the pin that the LED is attached to
//const byte pushButton = 2;
int distance;
int sensitivity = 70;
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
volatile int buttonState = 0;         // variable for reading the pushbutton status
volatile int sensor2State = 0;         // variable for reading the pushbutton status
long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis;
unsigned long currentMillis1;
long previousMillis1 = 0;
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 10000;           // interval at which to blink (milliseconds)
boolean dispensed = false;
boolean money = false;
boolean motorStarted = false;
boolean motorStopped = false;
boolean complete = false;
void setup()
    {
      Serial.begin(9600);
      pinMode(dispensedSensorPin, INPUT);
      pinMode(firstSensor, INPUT_PULLUP);
      pinMode(secondSensor, INPUT_PULLUP);
      pinMode(motor, OUTPUT);
      pinMode(buzzerPin,OUTPUT);

      strip.begin();
      strip.show();
    }

void loop()
    {
       currentMillis = millis();
       noteInserted(); 
       Serial.println(money);
       sensor2State = digitalRead(dispensedSensorPin);
  
       if(money == true )
         { 
           strip.setPixelColor(0,0,0,255);strip.show();
           motorStart();
           motorStarted = true;
           previousMillis = currentMillis; 
           Serial.println("previousMillis:");   
           Serial.println(previousMillis); 
           money = false;
           complete = false;
         }   
        if(sensor2State == LOW)
          {
            motorStop();
            motorStarted == false;
            motorStopped == true;
            strip.setPixelColor(0,0,0,0);strip.show();
            
          }
        if(motorStarted == true && motorStopped == false)
          {
            Serial.println("Check Time");
            if(currentMillis - previousMillis > interval && motorStarted == true && complete== false)
              {
                analogWrite(motor, 0);
                //dispensed = false;
                motorStarted == false;
                motorStopped == true;
                strip.setPixelColor(0,255,0,0);strip.show();
                
             Serial.println("forceSTOP");
             delay(3000);
             strip.setPixelColor(0,255,0,0);strip.show();
             
              }
          }   
             
        if(dispensed == true)
          { 
            Serial.println("dispensed");
            strip.setPixelColor(0,0,255,0);strip.show();
            delay(1000);
            beep(1000);
            delay(2000);
            strip.setPixelColor(0,0,0,0);strip.show();
            dispensed = false;
            money = false;
            complete = true;

          }
         
         
        
    }  


boolean noteInserted()
       {
         
         int firstSensorState = digitalRead(firstSensor); 
         int secondSensorState = digitalRead(secondSensor);

         
         if(firstSensorState == HIGH)
           { 
             reading = false;
             Serial.println("1");
             if(secondSensorState == LOW)
               {
                 Serial.println("2");
                  reading = true;
               }
           }   
         if(firstSensorState == LOW && reading == true)
           {
             
             Serial.println("3");       
             if(secondSensorState == HIGH && reading == true)
               {
                 Serial.println("4");
                 money = true;
                 reading = false;
               }
            }

       } 
      
void motorStop()
    {
      
      Serial.println("motor stop");
      analogWrite(motor, 0);
      dispensed = true; 
      motorStarted == false;
    } 

void motorStart()
    { 
      Serial.println("motor start");
      analogWrite(motor, 20);  
    } 
    
void beep(int delayValue)
     {
       digitalWrite(buzzerPin,HIGH);
       delay(delayValue);
       digitalWrite(buzzerPin,LOW);
     }    
     
