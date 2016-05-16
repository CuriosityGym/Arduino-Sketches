/*

  Clock using 12 LEDs

 In this example 12 LEDs are used to show current time.  
 12 LEDs are connected in circualr pattern. All 12 LEDs represent 
 number 1 to 12 of clock respectively. Depending upon the current 
 time, LED of that position will glow.
 If current time is 4:15 then 4th and 3rd LED will glow.
 4th LED will indicate hour and 3rd LED will indicate minutes.
 LED which shows hours will glow conitinuously. 
 LED which shows minutes will blink with delay of 500 miliseconds.

*/

int ledPins[] = {1,0,2,3,4,5,6,7,8,9,10,11};   //an array of pin numbers to which LEDs are attached
int pinCount = 12;  // the number of pins (i.e. the length of the array)

void setup() 
    { 
       for (int thisPin = 0; thisPin < pinCount; thisPin++)  // the array elements are numbered from 0 to (pinCount - 1).
          {
            pinMode(ledPins[thisPin],OUTPUT);   // use a for loop to initialize each pin as an output:
          }
    }
  
  //thhis is a list of int variables used in this clock program
int elapsedSeconds=0;
int hour=0;
int minute=0;
int initialHours = 01;//variable to initiate hours
int initialMinutes = 0;//variable to initiate minutes
int initialSeconds = 00;//variable to initiate seconds

 //this method is for elapsed time in seconds
 int seconds()
    {
       elapsedSeconds = initialHours * 3600;
       elapsedSeconds = elapsedSeconds + (initialMinutes*60);
       elapsedSeconds = elapsedSeconds + initialSeconds;
       elapsedSeconds = elapsedSeconds + (millis()/1000);
       return elapsedSeconds;
    }
//this method is for hours
int hours()
   {
        hour = seconds();
        hour = hour/3600;
        hour = hour % 24;
       return hour;
   }
//this method is for minutes
int minutes()
   {
       minute = seconds();
       minute = minute/60;
       minute = minute%60;
       return minute;
   }


//this loop will conintue to keep looping so that the time can go as follows
void loop()
   {
     digitalClockDisplay(); //by calling this function particular LED will turn ON and OFF depending upon time
    
   }

// this method is to turn ON and turn OFF particular LED according to time
void digitalClockDisplay()
   {
    int minuteLed = minutes() / 5; //dividing minutes by 5 will give the value between 1 to 12
    int hourLed = hours();    // houeLed varible will store the value of currnet hour
       
    if (minuteLed == hourLed)                   // if current hours and minutes are equal then 
       {                                        // then LED which will indicate hours and minutes 
          digitalWrite(ledPins[hourLed], HIGH); // which is same will blink with delay of 1 second
          delay(1000);                          
          digitalWrite(ledPins[hourLed], LOW);
          delay(1000);
       }
    else                                          // if current hours and minutes are diffrent
       {                                          // then LED which will indicate hour will 
          digitalWrite(ledPins[hourLed], HIGH);   // glow continously and LED which will indicate
          digitalWrite(ledPins[minuteLed], HIGH); // minutes will blink with delay of half second.
          delay(500);
          digitalWrite(ledPins[minuteLed], LOW);
          delay(500);       
       }
    }
