#include <Adafruit_NeoPixel.h>
#define PIN  6
#define NUMPIXELS  9
#define ROWS    3
#define COLS    3

int current_player = 1;
int InputPin[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
int tic_tac_toe[10]={ };
byte moves = 0;                                                
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); 
void setup() 
    {
      pixels.begin(); // This initializes the NeoPixel library.
      for(int i = 0; i < 9; i++)
         { 
           pinMode(InputPin[i], INPUT);
           pixels.setPixelColor(i, pixels.Color(0,0,250));
           pixels.show();
         }
      
    } 
 
void loop() 
    {
 
 
    }
   
void firstMove()
    { 
      if(current_player == 1)
        {
      for(int i = 0; i < 9; i++)
         { 
           if(digitalRead(InputPin[i]) == HIGH)
             {
              // char playerOne = 'x';
               pixels.setPixelColor(i, pixels.Color(0,0,250)); // blue color.
               pixels.show(); // This sends the updated pixel color to the hardware.
               tic_tac_toe[i] = 'x';     
               current_player = 0;  
            moves += 1;
             }    
             }
             
         }
     }
     
void nextMoves()     
     {
       if( moves%2 == 1)
         {
           for(int i = 0; i < 9; i++)
              { 
                if(digitalRead(InputPin[i]) == HIGH)
                  {
                    //char playerOne = 'o';
                    pixels.setPixelColor(i, pixels.Color(250,0,0)); //  red color.
                    pixels.show(); // This sends the updated pixel color to the hardware.
                    tic_tac_toe[i] = 'o';
                    current_player = 1;
                    moves += 1;
                  }
              }
         }
       else
         {
           for(int i = 0; i < 9; i++)
            { 
              if(digitalRead(InputPin[i]) == HIGH)
                {
                  //char playerOne = 'o';
                  pixels.setPixelColor(i, pixels.Color(0,0,250)); //  red color.
                  pixels.show(); // This sends the updated pixel color to the hardware.
                  tic_tac_toe[i] = 'x';
                  current_player = 1;
                  moves += 1;
                }
            }
         }
      }     

bool gameOver()
    {
    
        // Check rows
        if( tic_tac_toe[0]==tic_tac_toe[1]==tic_tac_toe[2])
          { if(tic_tac_toe[0] == 'x')
              { for(int i=0;i<5;i++)
                   {
                    pixels.setPixelColor(0, pixels.Color(0,0,250));
                    pixels.setPixelColor(1, pixels.Color(0,0,250));
                    pixels.setPixelColor(2, pixels.Color(0,0,250));
                    pixels.show();
                    delay(500);
                   }
                 ledEffects_P1();  
              }
             if(tic_tac_toe[0] == '0') 
              { 
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
           } 
         if( tic_tac_toe[3]==tic_tac_toe[4]==tic_tac_toe[5])
          { if(tic_tac_toe[3] == 'x')
              { for(int i=0;i<5;i++)
                   {
                    pixels.setPixelColor(3, pixels.Color(0,0,250));
                    pixels.setPixelColor(4, pixels.Color(0,0,250));
                    pixels.setPixelColor(5, pixels.Color(0,0,250));
                    pixels.show();
                    delay(500);
                   }
                 ledEffects_P1();  
              }
             if(tic_tac_toe[3] == '0') 
              { 
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
           }
         
         if( tic_tac_toe[6]==tic_tac_toe[7]==tic_tac_toe[8])
          { if(tic_tac_toe[6] == 'x')
              { for(int i=0;i<5;i++)
                   {
                    pixels.setPixelColor(6, pixels.Color(0,0,250));
                    pixels.setPixelColor(7, pixels.Color(0,0,250));
                    pixels.setPixelColor(8, pixels.Color(0,0,250));
                    pixels.show();
                    delay(500);
                   }
                ledEffects_P1();   
              }
             if(tic_tac_toe[6] == '0') 
              { 
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
          }       
            

        // Check columns
        
        if(tic_tac_toe[0]==tic_tac_toe[3]==tic_tac_toe[6])
          { if(tic_tac_toe[0] == 'x')
              { for(int i=0;i<5;i++)
                   {
                    pixels.setPixelColor(0, pixels.Color(0,0,250));
                    pixels.setPixelColor(3, pixels.Color(0,0,250));
                    pixels.setPixelColor(6, pixels.Color(0,0,250));
                    pixels.show();
                    delay(500);
                   }
                 ledEffects_P1();  
              }
             if(tic_tac_toe[0] == '0') 
              { 
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
           } 
         if( tic_tac_toe[1]==tic_tac_toe[4]==tic_tac_toe[7])
          { if(tic_tac_toe[1] == 'x')
              { for(int i=0;i<5;i++)
                   {
                    pixels.setPixelColor(1, pixels.Color(0,0,250));
                    pixels.setPixelColor(4, pixels.Color(0,0,250));
                    pixels.setPixelColor(7, pixels.Color(0,0,250));
                    pixels.show();
                    delay(500);
                   }
                ledEffects_P1();   
              }
             if(tic_tac_toe[1] == '0') 
              { 
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
           }
         
         if(tic_tac_toe[2]==tic_tac_toe[5]==tic_tac_toe[8])
          { if(tic_tac_toe[2] == 'x')
              { for(int i=0;i<5;i++)
                   {
                    pixels.setPixelColor(2, pixels.Color(0,0,250));
                    pixels.setPixelColor(5, pixels.Color(0,0,250));
                    pixels.setPixelColor(8, pixels.Color(0,0,250));
                    pixels.show();
                    delay(500);
                   }
                 ledEffects_P1();  
              }
             if(tic_tac_toe[2] == '0') 
              { 
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
          }      
        
    // Check diagonals
    if(tic_tac_toe[0]==tic_tac_toe[4]==tic_tac_toe[8])
          { if(tic_tac_toe[0] == 'x')
              { for(int i=0;i<5;i++)
                   {
                    pixels.setPixelColor(0, pixels.Color(0,0,250));
                    pixels.setPixelColor(4, pixels.Color(0,0,250));
                    pixels.setPixelColor(8, pixels.Color(0,0,250));
                    pixels.show();
                    delay(500);
                   }
                ledEffects_P1();   
              }
             if(tic_tac_toe[0] == '0') 
              { 
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
           } 
           
      if(tic_tac_toe[2]==tic_tac_toe[4]==tic_tac_toe[6])
          { if(tic_tac_toe[2] == 'x')
              { for(int i=0;i<5;i++)
                   {
                    pixels.setPixelColor(2, pixels.Color(0,0,250));
                    pixels.setPixelColor(4, pixels.Color(0,0,250));
                    pixels.setPixelColor(6, pixels.Color(0,0,250));
                    pixels.show();
                    delay(500);
                   }
                 ledEffects_P1();   
              }
             if(tic_tac_toe[2] == '0') 
              { 
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
    
     
