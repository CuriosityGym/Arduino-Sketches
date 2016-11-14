int ledPin1 = 3;    // LED connected to digital pin 3
int ledPin2 = 6;    // LED connected to digital pin 6
int ledPin3 = 9;    // LED connected to digital pin 9
int potentiometerPin = A2; 


void setup()  
    { 
     // nothing happens in setup 
    } 
 
void loop() 
    { 
      int potValue = potentiometerValue();
      int fadeValue = map(potValue, 0, 1023, 0, 255);   // sets the value (range from 0 to 255):
      
      analogWrite(ledPin1, fadeValue);
      analogWrite(ledPin2, fadeValue);
      analogWrite(ledPin3, fadeValue);        
      // wait for 30 milliseconds to see the dimming effect    
      delay(30);                            
    }

    
//function to calculate potentiometer value
int potentiometerValue()
   {
    int val = analogRead(potentiometerPin);
    return val;
   }    
