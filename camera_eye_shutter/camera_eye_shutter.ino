
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int ldrPin = A0;
int referenceValue = 512;
int pulse1;
int pulse2;
void setup() {
  //Serial.begin(9600);
  //Serial.println("start");
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
  //Serial.println("centre");
  myservo.writeMicroseconds(1470);
  delay(2000);
}

void loop() {
  int lightValue = analogRead(ldrPin);
 
  //myservo.writeMicroseconds(pulse);
  if (lightValue < referenceValue)
     {  //Serial.println("Low Light");
        if((referenceValue - lightValue) > 80)
          {//Serial.println("Low Light operation");
            pulse1 = (lightValue, 0,512,1470,1940);
            // Serial.print("Pulse1 : ");Serial.println(pulse1);
             myservo.writeMicroseconds(pulse1);
          }
     }
  if (lightValue > referenceValue)
     {//Serial.println("Bright Light");
         if((lightValue - referenceValue) > 80)
          {//Serial.println("Bright Light operation");
             pulse2 = (lightValue,512,1023,1470,1000);
             //Serial.print("Pulse2 : ");Serial.println(pulse2);
             myservo.writeMicroseconds(pulse2);
          }
     }   
}
