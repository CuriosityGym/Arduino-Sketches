
int endStop = 2;
int touchPad = 5;
int motorPin1 = 11;
int motorPin2 = 12;
int buzzerPin =13;
unsigned long currentMillis=0;
unsigned long previousMillis = 0;
unsigned long timeStamp=0;
unsigned long currentMillis1=0;
unsigned long previousMillis1 = 0;
boolean start = true;
boolean pM = true;
boolean pM1 = true;
boolean startMotor;

//stepper moto variables
// defines pins numbers
const int stepPin = 3; 
const int dirPin = 6; 
int ms1= 9;
int ms2 = 8;
int ms3 = 7;
boolean extrudeFilament = false;
boolean extrudeFilament1 = false;
int meter = 23200;  //23200 
int halfMeter = 11600;  //11600
//button counter variables
int BUTTON_PIN = 4;
//int BUTTON_PIN2 = 4;
int BUTTON_STATE;
int BUTTON2_STATE;
int LAST_BUTTON_STATE = LOW;
long LAST_DEBOUNCE_TIME = 0;
long DEBOUNCE_DELAY = 100;
int BUTTON_COUNTER = 0;
int IDLE_DELAY=5000;
int lastPressedMillis=0;

int count = 0;
int z = 1;
int a = 1;
//boolean extrudeFilament = false;
//int finalCount = 0;
unsigned long currentMillis2 = 0;
unsigned long previousMillis2 = 0;
boolean checkCount = false;

// notes in the melody:
int melody[] = {
  262, 196, 196, 220, 196, 0, 247, 262
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// the setup routine runs once when you press reset:
void setup()
    {
      // initialize serial communication at 9600 bits per second:
      Serial.begin(9600);
      // make the endStop's pin an input:
      pinMode(endStop, INPUT_PULLUP );
      pinMode(BUTTON_PIN, INPUT_PULLUP);
     // pinMode(BUTTON_PIN2, INPUT_PULLUP);
      pinMode(touchPad, INPUT_PULLUP);
      pinMode(motorPin1, OUTPUT);
      pinMode(motorPin2, OUTPUT);
      pinMode(buzzerPin, OUTPUT);
      pinMode(stepPin,OUTPUT); 
      pinMode(dirPin,OUTPUT);
      pinMode(ms1,OUTPUT);
      pinMode(ms2,OUTPUT);
      pinMode(ms3,OUTPUT);
    }

// the loop routine runs over and over again forever:
void loop() 
    {
     // currentMillis2 = millis();
      // read the input pin:
      int endstopState = digitalRead(endStop);
      int touchpadState = digitalRead(touchPad);
      int buttonState = digitalRead(BUTTON_PIN);
      // print out the state of the button:
      Serial.print("endstopState: ");
      Serial.println(endstopState);
     // Serial.print("touch Pad: ");
     // Serial.println(touchpadState);
    //  delay(1);        // delay in between reads for stability\

/*
      if(touchpadState == 1 && buttonState == 0)
        {
          //startMotor = true;
          finalCount = 1;
          extrudeFilament = true;
        }
      if(touchpadState == 1 && buttonState == 1)
        {
          //startMotor = true;
          finalCount = 2;
          extrudeFilament = true;
        }    
     
   */  
      if(touchpadState == 0 && buttonState == 0)
        {  
          digitalWrite(buzzerPin,HIGH);
          delay(250);
          digitalWrite(buzzerPin,LOW);
          extrudeFilament1 = true;
        }
       while(extrudeFilament1 == true && count < 50)
            {
             if(a!=count)
               {       
                 digitalWrite(ms1,HIGH);
                 digitalWrite(ms2,LOW);
                 digitalWrite(dirPin,HIGH); //Changes the rotations direction
  
                 // Makes 400 pulses for making two full cycle rotation
                 for(int x = 0; x <= meter; x++)
                    { 
                     // Serial.println("Extrude 1 meter filament ");
                     digitalWrite(stepPin,HIGH);
                     delayMicroseconds(500);
                     digitalWrite(stepPin,LOW);
                     delayMicroseconds(500);
                     if(x >= meter)
                       {
                        //extrudeFilament = false;
                        startMotor = true;
                        Serial.println("cut 1 meter filament ");
                        a=count; 
                       }
                   }  
                } 
             if(start == true && startMotor == true)
                {
                  currentMillis = millis();
                  if(pM == true)
                    {
                      previousMillis = currentMillis;
                      pM = false;
                    }
                  right();
                  Serial.println(" Cut Filament");
                  endstopState = digitalRead(endStop);
                  if(endstopState == 0)
                    { 
                     Serial.println(" Stop the motor");
                     stopMotor();
                     timeStamp = currentMillis - previousMillis;
                     start = false;
                     startMotor = false;
                     pM = true;  
                    }
                 } 
           if(start == false)
             {
              currentMillis1 = millis();
              if(pM1 == true)
                {
                  previousMillis1 = currentMillis1;
                  pM1 = false;
                }
              left();
              Serial.println("Reverse the motor");
              if((currentMillis1 - previousMillis1) > timeStamp)
                {
                  Serial.println(" Stop the motor again");
                  stopMotor();
                  start = true;
                  pM1 = true;
                  count++;
                 if(count == 50)
                   {
                     count = 0;
                     extrudeFilament1 = false;
                     playTone();
                   }  
                }
           }
    }   
   if(touchpadState == 0 && buttonState == 1)
     {
      digitalWrite(buzzerPin,HIGH);
      delay(250);
      digitalWrite(buzzerPin,LOW);
      extrudeFilament = true;
     }
   while(extrudeFilament == true && count < 50)
       {  
        digitalWrite(ms1,HIGH);
        digitalWrite(ms2,LOW);
        digitalWrite(dirPin,HIGH); //Changes the rotations direction
        if(z!=count)
          {
            // Makes 400 pulses for making two full cycle rotation
            for(int y = 0; y <= halfMeter; y++)
               { 
                 // Serial.println("Extrude 2 meter filament ");
                 digitalWrite(stepPin,HIGH);
                 delayMicroseconds(500);
                 digitalWrite(stepPin,LOW);
                 delayMicroseconds(500);
                 if(y >= halfMeter)
                   {
                     // extrudeFilament = false;
                     startMotor = true;
                     Serial.println("cut half meter filament ");
                     z=count;  
                   }
                           
               }     
           }
          
          if(start == true && startMotor == true)
            {
             currentMillis = millis();
             if(pM == true)
               {
                previousMillis = currentMillis;
                pM = false;
               }
             right();
             Serial.println(" Cut Filament");
             endstopState = digitalRead(endStop);
             if(endstopState == 0)
               { 
                Serial.println(" Stop the motor");
                stopMotor();
                timeStamp = currentMillis - previousMillis;
                start = false;
                startMotor = false;
                pM = true;
            
               }
            }
          if(start == false)
            {
             currentMillis1 = millis();
             if(pM1 == true)
               {
                previousMillis1 = currentMillis1;
                pM1 = false;
               }
             left();
             Serial.println("Reverse the motor");
             if((currentMillis1 - previousMillis1) > timeStamp)
               {
                 Serial.println(" Stop the motor again");
                 stopMotor();
                 start = true;
                 pM1 = true;
                 count++;
                 if(count == 50)
                   {
                    count = 0;
                    extrudeFilament = false;
                    playTone();
                   }    
                }
            }
                 
       }
   }

int right()
   {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
   }

int left()
   {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
   }

int stopMotor()
   {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
   }   
void playTone()
    {
       for (int thisNote = 0; thisNote < 8; thisNote++) 
            {
               // to calculate the note duration, take one second
               // divided by the note type.
               //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
               int noteDuration = 1000 / noteDurations[thisNote];
               tone(13, melody[thisNote], noteDuration);

               // to distinguish the notes, set a minimum time between them.
               // the note's duration + 30% seems to work well:
               int pauseBetweenNotes = noteDuration * 1.30;
               delay(pauseBetweenNotes);
               // stop the tone playing:
               noTone(13);
            }                   
    }
