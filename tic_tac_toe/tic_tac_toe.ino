#include <Adafruit_NeoPixel.h>
#define PIN  9
#define NUMPIXELS  9
boolean won = false;
int InputPin[9] = {0 ,2, 3, 4, 5, 6, 7, 8, 12};
int InputPinState[9] = {1,1,1,1,1,1,1,1,1};
int LastInputPinState[9] = {1,1,1,1,1,1,1,1,1};
char tic_tac_toe[9]={ };
byte moves = 0; 
int firstPlayer;
byte player1 = 10;
byte player2 = 11;

// notes in the melody:
int WinMelody[]= {659,587,370,415,523,494,294,329,494,440,277,329,440};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations1[] = {  8,8,4,4,8,4,4,4,8,8,4,4,4};

// notes in the melody:
int DrawMelody[]= {1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations2[] = {8,8,8,8,8,8,8,8,8,8};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); 
void setup() 
    {Serial.begin(9600);Serial.println("NEW GAME");
      pixels.begin(); // This initializes the NeoPixel library.
      for(int i = 0; i < 9; i++)
         { 
           pinMode(InputPin[i], INPUT_PULLUP);
           pixels.setPixelColor(i, pixels.Color(0,0,0));
           pixels.show();
         }
      pinMode(player1, OUTPUT);
      pinMode(player2, OUTPUT);
      first_XorO();
      Serial.println(first_XorO());
       
    } 
 
void loop() 
    {
      nextMoves();
      if(moves>=4 )
        { 
          winCondition();
           
        }  
        
      if (moves == 9 && !winCondition())
         {  
           moves = 0;
           tic_tac_toe[9]={ };
           for(int thisNote = 0; thisNote < 10; thisNote++)
              {
               // to calculate the note duration, take one second 
               // divided by the note type.
               //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
               int noteDuration2 = 1000/noteDurations2[thisNote];
               tone(A1, DrawMelody[thisNote],noteDuration2);
               //to distinguish the notes, set a minimum time between them.
               //the note's duration + 30% seems to work well:
               int pauseBetweenNotes = noteDuration2 * 1.60;
               delay(pauseBetweenNotes);
               // stop the tone playing:
               noTone(8);
              }
           for(int i = 0; i < 9; i++)
              { 
                pixels.setPixelColor(i, pixels.Color(0,250,0));
                pixels.show();
                delay(250);
              }
           
            delay(2000);
            for(int i = 0; i < 9; i++)
              { 
                pixels.setPixelColor(i, pixels.Color(0,0,0));
                pixels.show();
                delay(250);
              }
            delay(1000);
            Serial.print("draw");
         }
     }
 

int first_XorO()
    { randomSeed(analogRead(A5));
      firstPlayer = random(2);
      
      if(firstPlayer == 1)
        {
          digitalWrite(player1,HIGH);
          digitalWrite(player2,LOW);
        }
      if(firstPlayer == 0)
        {
          digitalWrite(player2,HIGH);
          digitalWrite(player1,LOW);
        } 
      return firstPlayer;  
    }

  
void nextMoves()     
     {
       if( moves%2 == 0 && moves < 9)
         {
           for(int i = 0; i < 9; i++)
              { 
                InputPinState[i] = digitalRead(InputPin[i]);
               if (InputPinState[i] != LastInputPinState[i]){
                if(InputPinState[i] == LOW)
                  {
                    if(firstPlayer== 1)
                      {
                       tic_tac_toe[i] = 'x';
                       pixels.setPixelColor(i, pixels.Color(0,0,250)); //  red color.
                       pixels.show(); // This sends the updated pixel color to the hardware.
                      }
                    if(firstPlayer == 0)
                      {
                       tic_tac_toe[i] = 'o';
                       pixels.setPixelColor(i, pixels.Color(250,0,0)); //  red color.
                       pixels.show(); // This sends the updated pixel color to the hardware.
                      }  
                     Serial.print(i);Serial.print( tic_tac_toe[i]);Serial.print("moves: ");Serial.println(moves);
                    moves += 1;
                    delay(500);
                  }}LastInputPinState[i] = InputPinState[i];
              }
         }
      if( moves%2 == 1 && moves < 9)
         {
           for(int i = 0; i < 9; i++)
            { InputPinState[i] = digitalRead(InputPin[i]);
              if (InputPinState[i] != LastInputPinState[i]){
              if(InputPinState[i] == LOW)
                {
                  if(firstPlayer == 1)
                      {
                       tic_tac_toe[i] = 'o';
                       pixels.setPixelColor(i, pixels.Color(250,0,0)); //  red color.
                       pixels.show(); // This sends the updated pixel color to the hardware.
                      }
                    if(firstPlayer == 0)
                      {
                       tic_tac_toe[i] = 'x';
                       pixels.setPixelColor(i, pixels.Color(0,0,250)); //  red color.
                       pixels.show(); // This sends the updated pixel color to the hardware.
                      }  
                  Serial.print(i);Serial.print( tic_tac_toe[i]);Serial.print("moves: ");Serial.println(moves);
                  moves += 1;
                  delay(500);
                }}LastInputPinState[i] = InputPinState[i];
            }
         }
     }     


boolean winCondition()
    {
       // Check rows
       if(tic_tac_toe[0]== 'x' && tic_tac_toe[1]== 'x' && tic_tac_toe[2]== 'x')
         { 
           Serial.println("x won  1st Row");
           for(int i=0;i<5;i++)
              {
               pixels.setPixelColor(0, pixels.Color(0,0,0));
               pixels.setPixelColor(1, pixels.Color(0,0,0));
               pixels.setPixelColor(2, pixels.Color(0,0,0));
               pixels.show();
               delay(250);
               pixels.setPixelColor(0, pixels.Color(0,0,250));
               pixels.setPixelColor(1, pixels.Color(0,0,250));
               pixels.setPixelColor(2, pixels.Color(0,0,250));
               pixels.show();
               delay(250);
              }
           ledEffects_P1();
           return true;  
          }
       if(tic_tac_toe[0]== 'o' && tic_tac_toe[1]== 'o' && tic_tac_toe[2]== 'o') 
         {
           Serial.println("o won  1st Row");
           for(int i=0;i<5;i++)
              {
               pixels.setPixelColor(0, pixels.Color(250,0,0));
               pixels.setPixelColor(1, pixels.Color(250,0,0));
               pixels.setPixelColor(2, pixels.Color(250,0,0));
               pixels.show();
               delay(250);
               pixels.setPixelColor(0, pixels.Color(0,0,0));
               pixels.setPixelColor(1, pixels.Color(0,0,0));
               pixels.setPixelColor(2, pixels.Color(0,0,0));
               pixels.show();
               delay(250);
              }
           ledEffects_P2(); 
           return true;  
         }    
        
       if(tic_tac_toe[3]== 'x' && tic_tac_toe[4]== 'x' && tic_tac_toe[5]== 'x')
         {
           Serial.println("x won  2nd Row");
           for(int i=0;i<5;i++)
              {
               pixels.setPixelColor(3, pixels.Color(0,0,0));
               pixels.setPixelColor(4, pixels.Color(0,0,0));
               pixels.setPixelColor(5, pixels.Color(0,0,0));
               pixels.show();
               delay(250);
               pixels.setPixelColor(3, pixels.Color(0,0,250));
               pixels.setPixelColor(4, pixels.Color(0,0,250));
               pixels.setPixelColor(5, pixels.Color(0,0,250));
               pixels.show();
               delay(250);
              }
           ledEffects_P1();  
           return true;
         }
       if(tic_tac_toe[3]== 'o' && tic_tac_toe[4]== 'o' && tic_tac_toe[5]== 'o') 
         { 
           Serial.println("o won  2nd Row");
           for(int i=0;i<5;i++)
              {
               pixels.setPixelColor(3, pixels.Color(250,0,0));
               pixels.setPixelColor(4, pixels.Color(250,0,0));
               pixels.setPixelColor(5, pixels.Color(250,0,0));
               pixels.show();
               delay(250);
               pixels.setPixelColor(3, pixels.Color(0,0,0));
               pixels.setPixelColor(4, pixels.Color(0,0,0));
               pixels.setPixelColor(5, pixels.Color(0,0,0));
               pixels.show();
               delay(250);
              }
           ledEffects_P2();
           return true;   
         }    
        
      if(tic_tac_toe[6]== 'x' && tic_tac_toe[7]== 'x' && tic_tac_toe[8]== 'x')
        { 
          Serial.println("x won  3rd Row");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(6, pixels.Color(0,0,0));
              pixels.setPixelColor(7, pixels.Color(0,0,0));
              pixels.setPixelColor(8, pixels.Color(0,0,0));
              pixels.show();
              delay(250);
              pixels.setPixelColor(6, pixels.Color(0,0,250));
              pixels.setPixelColor(7, pixels.Color(0,0,250));
              pixels.setPixelColor(8, pixels.Color(0,0,250));
              pixels.show();
              delay(250);
             }
          ledEffects_P1();
          return true;   
         
        }
      if(tic_tac_toe[6]== 'o' && tic_tac_toe[7]== 'o' && tic_tac_toe[8]== 'o') 
        {  
          Serial.println("o won  3rd Row");
          for(int i=0;i<5;i++)
             {
               pixels.setPixelColor(6, pixels.Color(250,0,0));
               pixels.setPixelColor(7, pixels.Color(250,0,0));
               pixels.setPixelColor(8, pixels.Color(250,0,0));
               pixels.show();
               delay(250);
               pixels.setPixelColor(6, pixels.Color(0,0,0));
               pixels.setPixelColor(7, pixels.Color(0,0,0));
               pixels.setPixelColor(8, pixels.Color(0,0,0));
               pixels.show();
               delay(250);
             }
          ledEffects_P2();
          
          return true;   
        }    
                     
      // Check columns
      if(tic_tac_toe[2]== 'x' && tic_tac_toe[3]== 'x' && tic_tac_toe[8]== 'x')
        { 
          Serial.println("x won  1st colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(2, pixels.Color(0,0,0));
              pixels.setPixelColor(3, pixels.Color(0,0,0));
              pixels.setPixelColor(8, pixels.Color(0,0,0));
              pixels.show();
              delay(250);
              pixels.setPixelColor(2, pixels.Color(0,0,250));
              pixels.setPixelColor(3, pixels.Color(0,0,250));
              pixels.setPixelColor(8, pixels.Color(0,0,250));
              pixels.show();
              delay(250);
             }
          ledEffects_P1();
          return true;  
        }
      if(tic_tac_toe[2]== 'o' && tic_tac_toe[3]== 'o' && tic_tac_toe[8]== 'o') 
        { 
          Serial.println("o won  1st colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(2, pixels.Color(250,0,0));
              pixels.setPixelColor(3, pixels.Color(250,0,0));
              pixels.setPixelColor(8, pixels.Color(250,0,0));
              pixels.show();
              delay(250);
              pixels.setPixelColor(2, pixels.Color(0,0,0));
              pixels.setPixelColor(3, pixels.Color(0,0,0));
              pixels.setPixelColor(8, pixels.Color(0,0,0));
              pixels.show();
              delay(250);
             }
          ledEffects_P2();
          return true;   
         }    
      if(tic_tac_toe[1]== 'x' && tic_tac_toe[4]== 'x' && tic_tac_toe[7]== 'x')
        { 
          Serial.println("x won  2nd colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(1, pixels.Color(0,0,0));
              pixels.setPixelColor(4, pixels.Color(0,0,0));
              pixels.setPixelColor(7, pixels.Color(0,0,0));
              pixels.show();
              delay(250);
              pixels.setPixelColor(1, pixels.Color(0,0,250));
              pixels.setPixelColor(4, pixels.Color(0,0,250));
              pixels.setPixelColor(7, pixels.Color(0,0,250));
              pixels.show();
              delay(250);
             }
          ledEffects_P1(); 
          return true;  
        }
      if(tic_tac_toe[1]== 'o' && tic_tac_toe[4]== 'o' && tic_tac_toe[7]== 'o') 
        { 
          Serial.println("o won  2nd colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(1, pixels.Color(250,0,0));
              pixels.setPixelColor(4, pixels.Color(250,0,0));
              pixels.setPixelColor(7, pixels.Color(250,0,0));
              pixels.show();
              delay(250);
              pixels.setPixelColor(1, pixels.Color(0,0,0));
              pixels.setPixelColor(4, pixels.Color(0,0,0));
              pixels.setPixelColor(7, pixels.Color(0,0,0));
              pixels.show();
              delay(500);
             }
          ledEffects_P2(); 
          return true;  
        }    
      if(tic_tac_toe[0]== 'x' && tic_tac_toe[5]== 'x' && tic_tac_toe[6]== 'x')
        { 
          Serial.println("x won  3rd colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(0, pixels.Color(0,0,0));
              pixels.setPixelColor(5, pixels.Color(0,0,0));
              pixels.setPixelColor(6, pixels.Color(0,0,0));
              pixels.show();
              delay(250);
              pixels.setPixelColor(0, pixels.Color(0,0,250));
              pixels.setPixelColor(5, pixels.Color(0,0,250));
              pixels.setPixelColor(6, pixels.Color(0,0,250));
              pixels.show();
              delay(250);
             }
          ledEffects_P1();
          return true;  
        }
      if(tic_tac_toe[0]== 'o' && tic_tac_toe[5]== 'o' && tic_tac_toe[6]== 'o') 
        { Serial.println("o won  3rd colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(0, pixels.Color(250,0,0));
              pixels.setPixelColor(5, pixels.Color(250,0,0));
              pixels.setPixelColor(6, pixels.Color(250,0,0));
              pixels.show();
              delay(250);
              pixels.setPixelColor(0, pixels.Color(0,0,0));
              pixels.setPixelColor(5, pixels.Color(0,0,0));
              pixels.setPixelColor(6, pixels.Color(0,0,0));
              pixels.show();
              delay(250);
             }
          ledEffects_P2();
          return true;   
        }    
        
     //check diagonals
     if(tic_tac_toe[0]== 'x' && tic_tac_toe[4]== 'x' && tic_tac_toe[8]== 'x')
       {
         Serial.println("x won  diagonal L");
         for(int i=0;i<5;i++)
            {
             pixels.setPixelColor(0, pixels.Color(0,0,0));
             pixels.setPixelColor(4, pixels.Color(0,0,0));
             pixels.setPixelColor(8, pixels.Color(0,0,0));
             pixels.show();
             delay(250);
             pixels.setPixelColor(0, pixels.Color(0,0,250));
             pixels.setPixelColor(4, pixels.Color(0,0,250));
             pixels.setPixelColor(8, pixels.Color(0,0,250));
             pixels.show();
              delay(250);
           }
        ledEffects_P1();
        return true;   
      }
    if(tic_tac_toe[0]== 'o' && tic_tac_toe[4]== 'o' && tic_tac_toe[8]== 'o') 
      { 
        Serial.println("o won  diagonal L");
        for(int i=0;i<5;i++)
           {
            pixels.setPixelColor(0, pixels.Color(250,0,0));
            pixels.setPixelColor(4, pixels.Color(250,0,0));
            pixels.setPixelColor(8, pixels.Color(250,0,0));
            pixels.show();
            delay(250);
            pixels.setPixelColor(0, pixels.Color(0,0,0));
            pixels.setPixelColor(4, pixels.Color(0,0,0));
            pixels.setPixelColor(8, pixels.Color(0,0,0));
            pixels.show();
            delay(250);
           }
        ledEffects_P2(); 
        return true; 
      } 
    if(tic_tac_toe[2]== 'x' && tic_tac_toe[4]== 'x' && tic_tac_toe[6]== 'x')
      { 
        Serial.println("x won  diagonal R");
        for(int i=0;i<5;i++)
           {
            pixels.setPixelColor(2, pixels.Color(0,0,0));
            pixels.setPixelColor(4, pixels.Color(0,0,0));
            pixels.setPixelColor(6, pixels.Color(0,0,0));
            pixels.show();
            delay(250);
            pixels.setPixelColor(2, pixels.Color(0,0,250));
            pixels.setPixelColor(4, pixels.Color(0,0,250));
            pixels.setPixelColor(6, pixels.Color(0,0,250));
            pixels.show();
            delay(250);
           }
        ledEffects_P1();
   
      }
    if(tic_tac_toe[2]== 'o' && tic_tac_toe[4]== 'o' && tic_tac_toe[6]== 'o') 
      { 
        Serial.println("o won  diagonal R");
        for(int i=0;i<5;i++)
           {
            pixels.setPixelColor(2, pixels.Color(250,0,0));
            pixels.setPixelColor(4, pixels.Color(250,0,0));
            pixels.setPixelColor(6, pixels.Color(250,0,0));
            pixels.show();
            delay(250);
            pixels.setPixelColor(2, pixels.Color(0,0,0));
            pixels.setPixelColor(4, pixels.Color(0,0,0));
            pixels.setPixelColor(6, pixels.Color(0,0,0));
            pixels.show();
            delay(250);
           }
        ledEffects_P2();
        return true;   
      }   
     return false;      
    }

void ledEffects_P1()
    { moves = 0;
      tic_tac_toe[9]={ };
      winningTone();     
      for(byte i=0;i<9;i++)
         {
           pixels.setPixelColor(i, pixels.Color(0,0,250));
           pixels.show();
           delay(100);
         }
         delay(2000);
      for(byte i=0;i<9;i++)
         {
           pixels.setPixelColor(i%10, pixels.Color(0,0,0));
           pixels.show();
           delay(100);
         }   
    }

void ledEffects_P2()
    { moves = 0;
      tic_tac_toe[9]={ };
      winningTone();
      for(byte i=0;i<9;i++)
         { 
           pixels.setPixelColor(i, pixels.Color(250,0,0));
           pixels.show();
           delay(100);
         }
         delay(2000);
      for(byte i=0;i<9;i++)
         { 
           moves = 0;
           tic_tac_toe[9]={ };
           pixels.setPixelColor(i%10, pixels.Color(0,0,0));
           pixels.show();
           delay(100);
         }    
}

void winningTone()
    {
       for(int thisNote = 0; thisNote < 13; thisNote++)
          {
           // to calculate the note duration, take one second 
           // divided by the note type.
           //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
           int noteDuration1 = 1000/noteDurations1[thisNote];
           tone(A1, WinMelody[thisNote],noteDuration1);
           //to distinguish the notes, set a minimum time between them.
           //the note's duration + 30% seems to work well:
           int pauseBetweenNotes = noteDuration1 * 1.10;
           delay(pauseBetweenNotes);
           // stop the tone playing:
           noTone(8);
          }
    }        
