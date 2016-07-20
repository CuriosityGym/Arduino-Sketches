#include <Adafruit_NeoPixel.h>
#define PIN  6
#define NUMPIXELS  9
//#define ROWS    3
//#define COLS    3

boolean current_player = true;
int InputPin[9] = {2, 3, 4, 5, 11, 7, 8, 9, 10};
int InputPinState[9] = {1,1,1,1,1,1,1,1,1};
int LastInputPinState[9] = {1,1,1,1,1,1,1,1,1};
char tic_tac_toe[10]={ };
byte moves = 0;                                                
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); 
void setup() 
    {Serial.begin(9600);Serial.println("NEW GAME");
      pixels.begin(); // This initializes the NeoPixel library.
      for(int i = 0; i < 9; i++)
         { 
           pinMode(InputPin[i], INPUT_PULLUP);
           pixels.setPixelColor(i, pixels.Color(0,0,250));
           pixels.show();
         }
      
    } 
 
void loop() 
    {
       //firstMove();
       nextMoves();
      if (moves>4){ gameOver();
      }
    }
/*   
void firstMove()
    { 
      if(current_player)
        {
      for(int i = 0; i < 9; i++)
         { InputPinState[i] = digitalRead(InputPin[i]);
           if(InputPinState[i] == LOW)
             {Serial.print(InputPin[i]);
               Serial.print(InputPinState[i]);
              //  playerOne = 'x';
               pixels.setPixelColor(i, pixels.Color(0,0,250)); // blue color.
               pixels.show(); // This sends the updated pixel color to the hardware.
               tic_tac_toe[i] = 'x'; 
               Serial.print(i);Serial.println( tic_tac_toe[i]);   
                 
               moves += 1;
               Serial.print("moves: ");Serial.print(moves);
             }    
             }
            current_player = false; 
         }
     }
  */   
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
                    //char playerOne = 'o';
                    pixels.setPixelColor(i, pixels.Color(250,0,0)); //  red color.
                    pixels.show(); // This sends the updated pixel color to the hardware.
                    tic_tac_toe[i] = 'x';
                     Serial.print(i);Serial.println( tic_tac_toe[i]);Serial.print("moves: ");Serial.println(moves);
                   // current_player = 1;
                    moves += 1;InputPinState[i] == HIGH;delay(500);
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
                  //char playerOne = 'o';
                  pixels.setPixelColor(i, pixels.Color(0,0,250)); //  red color.
                  pixels.show(); // This sends the updated pixel color to the hardware.
                  tic_tac_toe[i] = 'o';
                  Serial.print(i);Serial.println( tic_tac_toe[i]);Serial.print("moves: ");Serial.println(moves);
                  //current_player = 1;
                  moves += 1;InputPinState[i] == HIGH;delay(500);
                }}LastInputPinState[i] = InputPinState[i];
            }
         }
        if (moves == 9)
       { moves = 0; tic_tac_toe[10]={ };delay(5000);}
      }     


void gameOver()
    {
       // Check rows
       if(tic_tac_toe[0]== 'x' && tic_tac_toe[1]== 'x' && tic_tac_toe[2]== 'x')
         { 
           Serial.println("x won  1st Row");
           for(int i=0;i<5;i++)
              {
               pixels.setPixelColor(0, pixels.Color(0,0,250));
               pixels.setPixelColor(1, pixels.Color(0,0,250));
               pixels.setPixelColor(2, pixels.Color(0,0,250));
               pixels.show();
               delay(500);
              }
           ledEffects_P1();  
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
               delay(500);
              }
           ledEffects_P2();   
         }    
        
       if(tic_tac_toe[3]== 'x' && tic_tac_toe[4]== 'x' && tic_tac_toe[5]== 'x')
         {
           Serial.println("x won  2nd Row");
           for(int i=0;i<5;i++)
              {
               pixels.setPixelColor(3, pixels.Color(0,0,250));
               pixels.setPixelColor(4, pixels.Color(0,0,250));
               pixels.setPixelColor(5, pixels.Color(0,0,250));
               pixels.show();
               delay(500);
              }
           ledEffects_P1();  
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
               delay(500);
              }
           ledEffects_P2();   
         }    
        
      if(tic_tac_toe[6]== 'x' && tic_tac_toe[7]== 'x' && tic_tac_toe[8]== 'x')
        { 
          Serial.println("x won  3rd Row");
          for(int i=0;i<5;i++)
             {
               pixels.setPixelColor(6, pixels.Color(0,0,250));
               pixels.setPixelColor(7, pixels.Color(0,0,250));
               pixels.setPixelColor(8, pixels.Color(0,0,250));
               pixels.show();
               delay(500);
             }
          ledEffects_P1();   
        }
      if(tic_tac_toe[8]== 'o' && tic_tac_toe[7]== 'o' && tic_tac_toe[8]== 'o') 
        { 
          Serial.println("o won  3rd Row");
          for(int i=0;i<5;i++)
             {
               pixels.setPixelColor(6, pixels.Color(250,0,0));
               pixels.setPixelColor(7, pixels.Color(250,0,0));
               pixels.setPixelColor(8, pixels.Color(250,0,0));
               pixels.show();
               delay(500);
             }
          ledEffects_P2();   
        }    
                     
      // Check columns
      if(tic_tac_toe[0]== 'x' && tic_tac_toe[3]== 'x' && tic_tac_toe[6]== 'x')
        { 
          Serial.println("x won  1st colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(0, pixels.Color(0,0,250));
              pixels.setPixelColor(3, pixels.Color(0,0,250));
              pixels.setPixelColor(6, pixels.Color(0,0,250));
              pixels.show();
              delay(500);
             }
          ledEffects_P1();  
        }
      if(tic_tac_toe[0]== 'o' && tic_tac_toe[3]== 'o' && tic_tac_toe[6]== 'o') 
        { 
          Serial.println("o won  1st colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(0, pixels.Color(250,0,0));
              pixels.setPixelColor(3, pixels.Color(250,0,0));
              pixels.setPixelColor(6, pixels.Color(250,0,0));
              pixels.show();
              delay(500);
             }
          ledEffects_P2();   
         }    
      if(tic_tac_toe[1]== 'x' && tic_tac_toe[4]== 'x' && tic_tac_toe[7]== 'x')
        { 
          Serial.println("x won  2nd colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(1, pixels.Color(0,0,250));
              pixels.setPixelColor(4, pixels.Color(0,0,250));
              pixels.setPixelColor(7, pixels.Color(0,0,250));
              pixels.show();
              delay(500);
             }
          ledEffects_P1();   
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
              delay(500);
             }
          ledEffects_P2();   
        }    
      if(tic_tac_toe[2]== 'x' && tic_tac_toe[5]== 'x' && tic_tac_toe[8]== 'x')
        { 
          Serial.println("x won  3rd colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(2, pixels.Color(0,0,250));
              pixels.setPixelColor(5, pixels.Color(0,0,250));
              pixels.setPixelColor(8, pixels.Color(0,0,250));
              pixels.show();
              delay(500);
             }
          ledEffects_P1();  
        }
      if(tic_tac_toe[2]== 'o' && tic_tac_toe[5]== 'o' && tic_tac_toe[8]== 'o') 
        { Serial.println("o won  3rd colm");
          for(int i=0;i<5;i++)
             {
              pixels.setPixelColor(2, pixels.Color(250,0,0));
              pixels.setPixelColor(5, pixels.Color(250,0,0));
              pixels.setPixelColor(8, pixels.Color(250,0,0));
              pixels.show();
              delay(500);
             }
          ledEffects_P2();   
        }    
        
    //check diagonals
    if(tic_tac_toe[0]== 'x' && tic_tac_toe[4]== 'x' && tic_tac_toe[8]== 'x')
      {
        Serial.println("x won  diagonal L");
        for(int i=0;i<5;i++)
           {
            pixels.setPixelColor(0, pixels.Color(0,0,250));
            pixels.setPixelColor(4, pixels.Color(0,0,250));
            pixels.setPixelColor(8, pixels.Color(0,0,250));
            pixels.show();
            delay(500);
           }
        ledEffects_P1();   
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
            delay(500);
           }
        ledEffects_P2();  
      } 
    if(tic_tac_toe[2]== 'x' && tic_tac_toe[4]== 'x' && tic_tac_toe[6]== 'x')
      { 
        Serial.println("x won  diagonal R");
        for(int i=0;i<5;i++)
           {
            pixels.setPixelColor(2, pixels.Color(0,0,250));
            pixels.setPixelColor(4, pixels.Color(0,0,250));
            pixels.setPixelColor(6, pixels.Color(0,0,250));
            pixels.show();
            delay(500);
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
            delay(500);
           }
        ledEffects_P2();   
      }            
    }

void ledEffects_P1()
    {
      for(byte i=0;i<9;i++)
         {
           pixels.setPixelColor(i, pixels.Color(0,0,250));
           pixels.show();
         }
      for(byte i=0;i<9;i++)
         {
           pixels.setPixelColor(i%10, pixels.Color(0,0,0));
           pixels.show();
           delay(250);
         }   
    }

void ledEffects_P2()
    {
      for(byte i=0;i<9;i++)
         {
           pixels.setPixelColor(i, pixels.Color(250,0,0));
           pixels.show();
         }
      for(byte i=0;i<9;i++)
         {
           pixels.setPixelColor(i%10, pixels.Color(0,0,0));
           pixels.show();
           delay(250);
         }    
    }    
    
     
