#include <Servo.h>

//
Servo myservo;  // create servo object to control a servo
Servo myservo1;  // create servo object to control a servo
// twelve servo objects can be created on most boards

float tempC;
int reading;
int tempPin = A0;
int fanPin = 6;
int ldrPin = A3;
int buttonPin = 5;
int buttonCount=0;
int count = 0;
bool doorClosed = false;
int reedSwitch = 7;
int touchPad = 4;
int touchCount=0;
int ldr = 13;
int led1 = 12;
void setup()
{
analogReference(INTERNAL);
Serial.begin(9600);
pinMode(fanPin, OUTPUT);
pinMode(led1, OUTPUT);
pinMode(buttonPin, INPUT);
pinMode(reedSwitch, INPUT);
pinMode(touchPad, INPUT);
digitalWrite(fanPin, LOW);
 pinMode(ldr, INPUT);
myservo.attach(9);  // attaches the servo on pin 9 to the servo object
myservo1.attach(10);  // attaches the servo on pin 9 to the servo object
//dht.begin();
}

void loop()
{
int touchPadState = digitalRead(touchPad);  
int doorState = digitalRead(reedSwitch);  
int buttonState= digitalRead(buttonPin);
int lightValue = digitalRead(ldr);
delay(250);

/*
if(t > 26.00 )
  {
    digitalWrite(fanPin, HIGH);
    
  }
 
if(t <= 26.00 )
  {
    digitalWrite(fanPin, LOW);
  } 
*/
if(buttonState == 1) buttonCount++;
if(buttonCount%2==0)
  {
    myservo.write(90);
    doorClosed = false;    
  }
if(buttonCount%2==1 && doorClosed == false)
  {
    myservo.write(180);    
  } 
if(doorState == 1)
  {
     myservo.write(90);
     doorClosed = true; 
  }

  
if(touchPadState == 1) touchCount++;
if(touchCount%2==0)
  {
    myservo1.write(90);    
  }
if(touchCount%2==1)
  {
    myservo1.write(180);    
  }  

 if(lightValue == 1) digitalWrite(led1, HIGH);
 if(lightValue == 0) digitalWrite(led1, LOW); 
}
