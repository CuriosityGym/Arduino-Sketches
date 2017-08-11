
int buzzerPin = 2;
// the setup function runs once when you press reset or power the board
void setup()
    {
       // initialize buzzer pin as an output.
       pinMode(buzzerPin, OUTPUT);
    
       delay(20000);
    }

// the loop function runs over and over again forever
void loop() 
    {
       digitalWrite(buzzerPin, HIGH);   // turn the buzzer on (HIGH is the voltage level)
       delay(1000);              // wait for a second
       digitalWrite(buzzerPin, LOW);    // turn the buzzer off by making the voltage LOW
       delay(1000);              // wait for a second
    }
