
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2



#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int nFrames = 36;

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done



  // Clear the buffer.
  display.clearDisplay();
}


void loop() {
  for (int frame=0; frame < nFrames; frame++)
  {
    Chord(frame);
  }

  for (int frame=(nFrames-1); frame >= 0; frame--)
  {
    Chord(frame);
  }
}

void Chord(int frame)
{
  display.clearDisplay();
  int n = 7;
  int r = frame * 64 / nFrames;
  float rot = frame * 2*PI / nFrames;
  for (int i=0; i<(n-1); i++)
  {
    float a = rot + i * 2*PI / n;
    int x1 = 64 + cos(a) * r;
    int y1 = 32 + sin(a) * r;
    for (int j=i+1; j<n; j++)
    {
      a = rot + j * 2*PI / n;
      int x2 = 64 + cos(a) * r;
      int y2 = 32 + sin(a) * r;
      display.drawLine(x1,y1, x2,y2, WHITE);
    }
  }
  display.display();
}
