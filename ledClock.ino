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

int led_Pins[] = {0,1,2,3,4,5,6,7,8,9,10,11};   //LED pins are stored in array
int pin_Count = 12;  // Total no of LEDs

void setup() 
  {
   // put your setup code here, to run once:
   for ( int i = 0; i > pin_Count; i++)     // set all LEDs to output
       {
        pinMode(led_Pins[i],OUTPUT);
       }
  }

void loop() 
  {
   // put your main code here, to run repeatedly:
   int time = 0;  // this variable will store number of minutes passed 
   unsigned long elapsedMinutes = millis()/1000; // we used unsigned long because after a few hours, the value of millis() will be huge!
   while(((millis()/1000) - elapsedMinutes) < 60);  //  this loop that will do nothing until a minute has passed. 
         time++;    //after every minute increment time 
 
   if(time >=720)   // this is 12 hour clock so after 720 minute
      time = 0;     // it will set to 0
 
   int hour = time/60;    // time/60 will give the the current value of hour
   int hour_FirstDigit = hour/10;     //  forward slash / to denote division, hour/10 will give the 
                                      //  value of quotient which is first digit of hour. 
   int hour_SecondDigit = hour%10;    //  modulo function '%' gives the value of remainder, which 
                                      //  is second digit of hour 
 
   int minute = time%60;               //  time%60 will give the current value of minutes
   int minute_FirstDigit = minute/10;  //  forward slash / to denote division, hour/10 will give the 
                                        //  value of quotient which is first digit of minutes.
   int minute_SecondDigit = minute%10; //  modulo function '%' gives the value of remainder, which 
                                        //  is second digit of minutes.
 
   if(hour_FirstDigit + hour_SecondDigit == 0)  // As this is 12 hour clock, at 12 o'clock it will show 00:00 
    {                                           // to make it 12:00, set the hour's first digit to 1 and    
     hour_FirstDigit = 1;                       // hour's second digit to 2.
     hour_SecondDigit = 2;
    }

  
   int hour_led = (hour_FirstDigit *10) + hour_SecondDigit;      // multiply the first digit of hour by 10 and add second digit of hour,
                                                                 // this will give number between 1 to 12.
   int minutes = (minute_FirstDigit * 10) + minute_SecondDigit;  // multiply the first digit of minutes by 10 and add second digit of minutes
                                                                 // this will give number between 1 to 60
   int minute_led = minutes/5;  //dividing minutes by 5 will give the value between 1 to 12

   digitalWrite(led_Pins[hour_led - 1], HIGH); // we have array of length 12 (0 to 11), we have 12 LEDs, 
                                               //to Turn ON the hour LED we need values between 0 to 11 
                                               //which can get by substracting 1 from value of current hour
   
   digitalWrite(led_Pins[minute_led - 1],HIGH); // substracting 1 from value of minute_led will turn on particular minute LED from array.
   delay(500);                                  // Turns on an LED on for one second,    
   digitalWrite(led_Pins[minute_led - 1],LOW);  // then off for half second, repeatedly.
   delay(500);
  
  }
