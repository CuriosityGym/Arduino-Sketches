#include <Servo.h> 
 
Servo myservo; 
int pos = 90;   // initial position
int sens1 = A0; // LDR 1 pin
int sens2 = A1; //LDR 2 pin
int tolerance = 50;
int lowEnd=60;
int HighEnd=120;
 
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo.write(pos);// Set Servo to 90 Degrees
  delay(2000); // a 2 seconds delay while we position the solar panel
  
}  
 int val1, val2;
void loop() 
{ 
  val1 = analogRead(sens1); // read the value of sensor 1  
  val2 = analogRead(sens2); // read the value of sensor 2

 
  if((abs(val1 - val2) <= tolerance) || (abs(val2 - val1) <= tolerance)) 
  {
    //do nothing if the difference between values is within the tolerance limit
  } 
  else 
  {    
    if(val1 > val2)
    {
      pos = --pos;
    }
    if(val1 < val2) 
    {
      pos = ++pos;
    }
  }
 
  if(pos > HighEnd) { pos = HighEnd; } // reset to 180 if it goes higher
  if(pos < lowEnd) { pos = lowEnd; } // reset to 0 if it goes lower
 
  myservo.write(pos); // write the position to servo
  delay(10);
}
