#include "LedControl.h"
byte leftArrow[8]={0x00,0x08,0x0c,0xfe,0xff,0xfe,0x0c,0x08};
byte rightArrow[8]={0x00,0x10,0x30,0x7f,0xff,0x7f,0x30,0x10};
LedControl lc=LedControl(12,11,10,1);
void setup() {
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

}

void loop() {
 for(int i=0;i<8;i++)
 {
    lc.setRow(0,i,leftArrow[i]);
 }
delay(1000);
  for(int i=0;i<8;i++)
 {
    lc.setRow(0,i,rightArrow[i]);
 }
 delay(1000);

}
