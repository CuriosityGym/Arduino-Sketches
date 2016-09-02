#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

int paddle0Pin = A3; // Left player potentiometer
int paddle1Pin = A2; // Right player potentiometer
int winningScore = 5; // How high before we declare a winner?
byte tonePin = A1; // Which digital pin the beeper is attached to
int animationSpeed0 = 25; // Initial speed of each round (lower = faster)

//== Game Variables ==
u8g_uint_t courtWidth, courtHeight, halfCourtWidth;
u8g_uint_t ballSize = 2;
u8g_uint_t ballX;
u8g_uint_t ballDirectionX = 1;
u8g_uint_t ballY;
u8g_uint_t ballDirectionY = 1;
u8g_uint_t paddleWidth = 3;
u8g_uint_t paddleHeight = 10;
u8g_uint_t paddleHalfHeight = paddleHeight/2;
u8g_uint_t paddle0Y; // Left player vertical paddle position
u8g_uint_t paddle1Y; // Right player vertical paddle position

int score0, score1; // Left & Right player's scores
bool gameOver = false;
char strScore0[] = "?"; // Sets string length to 1
char strScore1[] = "?";
  
int bounceToneFrequency = 523;
int bounceToneDuration = 62;
int missToneFrequency = 523 / 2;
int missToneDuration = 512;

unsigned long timeToMove; // When should we move the ball again?
int animationSpeed = animationSpeed0; // Current ball speed (lower = faster)

#define winner_width 30
#define winner_height 30
static const unsigned char winner[] PROGMEM= {
0x78, 0x00, 0x80, 0x07, 0x58, 0xFF, 0xBF, 0x06, 0xD8, 0xFF, 0xFF, 0x02, 
  0x10, 0xFF, 0x3F, 0x06, 0x18, 0xFF, 0x3F, 0x06, 0x10, 0xFE, 0x1F, 0x02, 
  0x10, 0xFE, 0x1F, 0x02, 0x30, 0xFE, 0x1F, 0x03, 0x30, 0xFE, 0x1F, 0x03, 
  0x20, 0xFE, 0x1F, 0x01, 0x60, 0xFE, 0x9F, 0x01, 0x40, 0xFC, 0x8F, 0x00, 
  0x80, 0xFC, 0x4F, 0x00, 0x80, 0xFD, 0x7F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 
  0x00, 0xFC, 0x0F, 0x00, 0x00, 0xF8, 0x07, 0x00, 0x00, 0xF0, 0x03, 0x00, 
  0x00, 0xE0, 0x01, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0xE0, 0x01, 0x00, 
  0x00, 0xF0, 0x01, 0x00, 0x00, 0xF8, 0x07, 0x00, 0x00, 0xFC, 0x0F, 0x00, 
  0x00, 0xFC, 0x0F, 0x00, 0x00, 0xFE, 0x1F, 0x00, 0x00, 0xFE, 0x1F, 0x00, 
  0x00, 0xFE, 0x1F, 0x00, 0x00, 0xFE, 0x1F, 0x00, 0x00, 0xFE, 0x1F, 0x00,  
 };
 
bool MissedPaddle(u8g_uint_t py)
{
  u8g_uint_t ballTop = ballY;
  u8g_uint_t ballBottom = ballY + ballSize - 1;
  u8g_uint_t paddleTop = py;
  u8g_uint_t paddleBottom = py + paddleHeight - 1;
  return ballBottom < paddleTop || ballTop > paddleBottom;
}

void DrawScores()
{
//  char strScore0[] = "?"; // Sets string length to 1
//  char strScore1[] = "?";
  strScore0[0] = '0' + score0; // Overide the string value with single digit score
  strScore1[0] = '0' + score1;
  u8g.setFont(u8g_font_timB08);
  u8g_uint_t scoreWidth = u8g.getStrPixelWidth(strScore0);
  const int offset = 5;
  u8g_uint_t scoreY = 9;
  u8g.drawStr( halfCourtWidth - offset - scoreWidth, scoreY, strScore0);
  u8g.drawStr( halfCourtWidth + offset, scoreY, strScore1);
  u8g.drawStr( 28,scoreY,"P1");
  u8g.drawStr( 90,scoreY,"P2");
}

void DrawGameOver()
{
  u8g.setFont(u8g_font_timB18);
  u8g.setFontPosCenter(); // vertical alignment
  char gameStr[] = "Game";
  char overStr[] = "Over";
  u8g_uint_t gx = (courtWidth - u8g.getStrPixelWidth(gameStr)) / 2;
  u8g_uint_t ox = (courtWidth - u8g.getStrPixelWidth(overStr)) / 2;
  u8g.drawStr(gx, 20, gameStr);
  u8g.drawStr(ox, 40, overStr);
  if(strScore0[0]> strScore1[0])
    {
     u8g.drawXBMP( 8, 32, winner_width, winner_height, winner);
     u8g.drawStr(110, 10, strScore1);
     u8g.drawStr(10, 10, strScore0);
    }
  else
    {
     u8g.drawXBMP( 92, 32, winner_width, winner_height, winner);
     u8g.drawStr(110, 10, strScore1);
     u8g.drawStr(10, 10, strScore0);
    }  
}

void DrawCourt()
{
  //u8g.drawHLine(0, 0, courtWidth);
  //u8g.drawHLine(0, courtHeight - 1, courtWidth);
  byte dash = 3;
  for (byte y = 0; y < (courtHeight / dash / 2); y++)
  {
    u8g.drawVLine(halfCourtWidth - 1, 2 + y * dash * 2, dash);
  }
}

void Player0Missed()
{
  // When left player missed, move the ball just to the left of rightmost of court
  ballX = courtWidth - ballSize - 1;
  ballY = paddle1Y + paddleHalfHeight; // ball will be served at location of player 1's paddle
  tone(tonePin, missToneFrequency, missToneDuration);
  delay(1000);
  score1++;
  animationSpeed = animationSpeed0;
  if (score1 == winningScore) gameOver = true;
}

void Player1Missed()
{
  // When right player missed, move the ball just to the right of the leftmost of court
  ballX = 1;
  ballY = paddle0Y + paddleHalfHeight; // ball will be served at location of player 0's paddle
  tone(tonePin, missToneFrequency, missToneDuration);
  delay(1000);
  score0++;
  animationSpeed = animationSpeed0;
  if (score0 == winningScore) gameOver = true;
}

void BounceX()
{
  tone(tonePin, bounceToneFrequency, bounceToneDuration);
  ballDirectionX = -ballDirectionX;
  animationSpeed--; // Speed up game with each bounce
}

void MoveBall()
{
  if (millis() > timeToMove) {
    ballX += ballDirectionX;
    if (ballX <= 0)
        if (MissedPaddle(paddle0Y)) Player0Missed(); else BounceX();

    if (ballX >= (courtWidth - ballSize))
        if (MissedPaddle(paddle1Y)) Player1Missed(); else BounceX();

    ballY += ballDirectionY;
    if (ballY >= (courtHeight - ballSize) || ballY <= 0) {
      ballDirectionY = -ballDirectionY;
      animationSpeed--; // Speed up game with each bounce
      tone(tonePin, bounceToneFrequency, bounceToneDuration);
    }

    timeToMove = millis() + animationSpeed;
  }
}

void DrawPaddle(u8g_uint_t paddleX, int paddleY)
{
  u8g.drawBox(paddleX, paddleY, paddleWidth, paddleHeight);
}

void DrawPaddles()
{
  paddle0Y = map(analogRead(paddle0Pin), 0, 1023, 0, courtHeight - paddleHeight);
  paddle1Y = map(analogRead(paddle1Pin), 0, 1023, 0, courtHeight - paddleHeight);

  DrawPaddle(0, paddle0Y);
  DrawPaddle(courtWidth - paddleWidth, paddle1Y);
}

void setup(void) {
  //u8g.setRot180();  // flip screen
  courtWidth = u8g.getWidth();
  courtHeight =  u8g.getHeight();
  halfCourtWidth = courtWidth / 2;
}

void loop(void) {
  u8g.firstPage();
  do {

    if (gameOver)
      DrawGameOver();
    else
      MoveBall();

    DrawCourt();
    DrawScores();
    u8g.drawDisc(ballX,ballY,ballSize);
    DrawPaddles();
  } while ( u8g.nextPage() );
}
