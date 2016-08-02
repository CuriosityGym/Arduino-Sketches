
const byte led_Pin_Count = 12; // // the number of leds (i.e. the length of the array)
byte led_Pins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; //an array of pin numbers to which LEDs are attached
int potentiometerPin = A2;   // The LDR is connected to analog pin 3
int minSpeed = 500; // put a high value
int maxSpeed = 50;  // put a low value
 
void setup()
    {
       for(int i = 0; i < led_Pin_Count; i++)   // set all LEDs to output
          {
	    pinMode(led_Pins[i], OUTPUT);
	  }  
    }
 
void loop()
    {       
       int pot_Val = potentiometerValue();   //this variable will store the ldr value
       for(int i = 0; i < led_Pin_Count; i++) 
          {
	    digitalWrite(led_Pins[i], HIGH);
            int speed = map(pot_Val, 0, 1023, minSpeed, maxSpeed);  //map the ldr value to minimum and maximum speed
            delay(speed);
	    digitalWrite(led_Pins[i], LOW);
	  }
    }

//function to calculate potentiometer value
int potentiometerValue()
   {
    int val = analogRead(potentiometerPin);
    return val;
   } 
