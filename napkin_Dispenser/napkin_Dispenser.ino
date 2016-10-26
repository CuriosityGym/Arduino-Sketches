#include <Adafruit_NeoPixel.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
int buzzerPin = A1;

const byte dispensedSensorPin = 3;  // This is our sensor2 pin
const byte moneySensorPin = 4;  // This is our sensor1 pin
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
void setup()
    {
      Serial.begin(9600);
      pinMode(dispensedSensorPin, INPUT);
      pinMode(moneySensorPin, INPUT);

      pinMode(motor, OUTPUT);
      //pinMode(pushButton, INPUT);
      pinMode(buzzerPin,OUTPUT);
      digitalWrite(moneySensorPin, HIGH);
     // attachInterrupt(1, motorStop, RISING);
      //attachInterrupt(0, motorStart, RISING);
      strip.begin();
      strip.show();
    }

void loop()
    {
       
       noteInserted(); 
       //Serial.println(money);
       sensor2State = digitalRead(dispensedSensorPin);
       if(money == true)
         {
           strip.setPixelColor(0,0,0,255);strip.show();    
         }
       
       if(money == true )
         { 
           strip.setPixelColor(0,0,0,255);strip.show();
           motorStart();
           motorStarted = true;
           previousMillis = currentMillis; 
           Serial.println("previousMillis:");   
           Serial.println(previousMillis); 
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
            currentMillis = millis();
            if(currentMillis - previousMillis > interval && motorStopped == false)
              {
                analogWrite(motor, 0);
                //dispensed = false;
                motorStarted == false;
                motorStopped == true;
               // strip.setPixelColor(0,255,0,0);strip.show();
                
             Serial.println("forceSTOP");
            // delay(3000);
            // strip.setPixelColor(0,255,0,0);strip.show();
             
              }
          }   
             
        if(dispensed == true)
          { 
            // if(currentMillis - previousMillis > interval)
            // {
             //   save the last time you blinked the LED 
              // previousMillis = currentMillis;
              strip.setPixelColor(0,0,255,0);strip.show();
              delay(1000);
              beep(1000);
              delay(2000);
              strip.setPixelColor(0,0,0,0);strip.show();
               
               //digitalWrite(greenLed, LOW);
               dispensed = false;
               money = false;
               
            //  }
            // else
            //   digitalWrite(greenLed, LOW); 
          }//*/
         
         
        
    }  

boolean noteInserted()
       { // digitalWrite(redLed, LOW);
          distance = digitalRead(moneySensorPin);
          if(distance ==  HIGH)
            {
              strip.setPixelColor(0,0,0,255);strip.show();
              //digitalWrite(blueLed, HIGH);
              //digitalWrite(redLed, LOW);
              money = true;
            }
          else
            { 
             strip.setPixelColor(0,0,0,0);strip.show();
            //  money = false;
            }  
          Serial.println(distance); 
          //return money;
       }


      
void motorStop()
    {
      
      Serial.println("motor stop");
      analogWrite(motor, 0);
      dispensed = true; 
    } 

void motorStart()
    { 
      //if(money == true)
      //  {
        // sensor1State = digitalRead(moneySensorPin);
        // Serial.println(sensor1State);
         Serial.println("motor start");
         analogWrite(motor, 20);
       //  money = false;
       // }    
    } 
    
void beep(int delayValue)
     {
       digitalWrite(buzzerPin,HIGH);
       delay(delayValue);
       digitalWrite(buzzerPin,LOW);
     }    
     
