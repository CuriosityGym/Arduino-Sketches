/*
   Automatic Garrage Door Opener
  
  PIR Sensor :
      PIR sensors allow you to sense motion, almost always used to detect whether a human has moved in or out of the sensors range.
      They are often referred to as PIR, "Passive Infrared", "Pyroelectric", or "IR motion" sensors.

  This example uses a motion-detecting sensor (PIR sensor) to open or close the door which detects the infrared energy omitted from human's body. 
  When someone comes in front of the door, the infrared energy detected by the sensor changes and it triggers the sensor to open the door whenever
  someone approaches the door. The signal is further sent to arduino uno that controls servo motors which are attached to the the door.

*/


#include<Servo.h> 
Servo myservo1;     //creates a servo object
Servo myservo2;     //a maximum of eight servo objects can be created
int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int pir_Value = 0;              // variable for reading the pin status
int motionDetected = 0;
int pos = 0;                    // initial servo position
void setup() 
    {
      pinMode(ledPin, OUTPUT);      // declare LED as output
      pinMode(inputPin, INPUT);     // declare sensor as input
      myservo1.attach(4);           //attaches servo1 to pin 4
      myservo2.attach(5);           //attaches servo2 to pin 5
      Serial.begin(9600);
    }
 
void loop()
    {
      pir_Value = digitalRead(inputPin);  // read input value
      if (pir_Value == HIGH) 
         { // check if the input is HIGH
           digitalWrite(ledPin, HIGH);  // turn LED ON
           if (pirState == LOW) 
              {
                // we have just turned on
                Serial.println("Motion detected!");  //print message on serial monitor
                motionDetected += 1;    //increment this counter by 1
                Serial.println(motionDetected);
                for(pos; pos < 180; pos += 1)  //goes from 0 to 180 degrees
                   {                                   //in steps of one degree
                     myservo1.write(pos);              //tells servo to go to position in variable "pos"
                     myservo2.write(180-pos);
                     delay(5);                         //waits for the servo to reach the position
                   }
                Serial.println("Door Opened");
                pirState = HIGH;
              }
          } 
       else 
          {
            digitalWrite(ledPin, LOW); // turn LED OFF
            if (pirState == HIGH)
               {
                 //  we have just turned of
                 Serial.println("Motion ended!");
                 // We only want to print on the output change, not state
                 for(pos = 180; pos>=1; pos-=1) //goes from 180 to 0 degrees
                    {                               
                      myservo1.write(pos);        //to make the servo go faster, decrease the time in delays for
                      myservo2.write(180-pos);
                      delay(5);                   //to make it go slower, increase the number.
                     }
                 Serial.println("Door Closed");
                 pirState = LOW;
               }
          }
    }
