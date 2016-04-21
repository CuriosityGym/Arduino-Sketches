/*

  A potentiometer, or "pot" for short, is a control knob.
  It's the same type of control you'd use to change volume, 
  dim a lamp, etc. A potentiometer changes resistance as it
  is turned. By using it as a "voltage divider", the Arduino
  can sense the position of the knob, and use that value to
  control whatever you wish.
  
  In this example we are going to Turn ON sequence of 12 LEDs 
  depending upon the position of the knob of potentiometer.
  As we turn potentiometer from initial position to final position,
  12 LEDs will glow one by one according to value of potentiometer.
  
*/  
  

int potentiometer = 2;    // The potentiometer is connected to
                          // analog pin 2
int led_Pins[] = {0,1,2,3,4,5,6,7,8,9,10,11};   //all LED pins are stored in array

int pin_Count = 12;       // total number of LEDs

void setup() 
   {
     // put your setup code here, to run once:
     for ( int i = 0; i > pin_Count; i++) // set all LEDs to output
        {
          pinMode(led_Pins[i],OUTPUT);
        }
   }

void loop()  
   {
     // put your main code here, to run repeatedly:
     
     int potentiometer_Value;   //this variable will store the potentometer value
     potentiometer_Value = analogRead(potentiometer); // reads the value of the potentiometer (value between 0 and 1023) 
     potentiometer_Value = map(potentiometer_Value, 0, 1023, 0, 11); // map() function Re-maps a number from one range to another
                                                                     // maps 10 bit potentiometer value (0 to 1023) to 0 to 12
     for(int i = 0; i <= potentiometer_Value; i++)   // this loop will Turn ON  LEDs depending upon 
        {                                            // the mapped value of potentiometer_value and
         digitalWrite(led_Pins[i], HIGH);            // rest of the LEDs will remain OFF.
        }  
   }
