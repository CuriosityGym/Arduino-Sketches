
int buzzerPin = A1;

int counter;
int light_threshold = 512;
boolean diaryRemoved = false;
void setup()
    {
      Serial.begin(9600);
      pinMode(buzzerPin,OUTPUT);
     
    }
  
void loop()
    {  
      int light_value1 = analogRead(A3); delay(10);
      int light_value = analogRead(A3);delay(10);  
      Serial.println(light_value);
      if(light_value > light_threshold && diaryRemoved == false)    //if there is change in light value (New Package)
        { 
          for(int i=0; i<10; i++)
             {
               beep(500);
               delay(500);
             }             
          Serial.print("Diary Removed!!");
          diaryRemoved = true;
                   
        }
        delay(2000);
     if(light_value < light_threshold && diaryRemoved == true)
       {
         diaryRemoved = false;
       }  
    }


void beep(int delayValue)
     {
       digitalWrite(buzzerPin,HIGH);
       delay(delayValue);
       digitalWrite(buzzerPin,LOW);
     }
