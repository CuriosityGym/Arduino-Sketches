

#include "Arduino.h"
#include "CGShield.h"
#include "../Wire/Wire.h"
//#include <Wire.h>




boolean DEBUG = false;
boolean RGB_CC = true;

byte THIS_ADDRESS;
byte THAT_ADDRESS;

// CG Shield Pin Outs
byte datapin    = 4; 
byte clockpin   = A4;
byte latchpin   = 2;  
byte data 		= 0;  // global data to send to shift register

byte lightPin    = A3;
byte trimPin   = A2;
byte speakerPin    = A1;
byte tempPin  = A0; 

byte motorPin   = 3;
byte IRPin   = 3;


byte buttonPinR = 4;  // right side
byte buttonPinL = 5;  // left side
byte RGBPin = 6;
byte touchPin   = A3;
byte analogPin=A5;	
int analogValue;
int trimValue;
int tempValue;
byte buttonLvalue;
byte buttonRvalue;
byte package[8];
long lastUpdate = 0;	
byte pointer = 0;	 // points to what data to return
byte MSB, LSB;


FunShield::FunShield(){
  FunShield(0, 0); // no I2C needed.
}

	
FunShield::FunShield(byte ME, byte FRIEND){

  THIS_ADDRESS = ME;
  THAT_ADDRESS = FRIEND;
 
  // don't know a better way to load these arrays
  //RED[0] = 255; RED[1] = 0;  RED[2] = 0;
  //GREEN[0] = 0; GREEN[1] = 255;  GREEN[2] = 0;
  //BLUE[0] = 0; BLUE[1] = 0;  BLUE[2] = 255;
  
  
  // Set the three SPI pins to be outputs:
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);  
  pinMode(latchpin, OUTPUT);
  
  // turn all off
  shiftOut(datapin, clockpin, MSBFIRST, 0);

  // Toggle the state of the latchPin,
  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin, LOW); 

  // disable digital13
  pinMode(13, INPUT);
  digitalWrite(13, LOW);
  
  // set input pins
  pinMode(trimPin, INPUT);
  pinMode(analogPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(buttonPinR, INPUT);
  pinMode(buttonPinL, INPUT);
  
  // engage pull ups
  digitalWrite(lightPin, HIGH);
  digitalWrite(analogPin, HIGH);
  digitalWrite(buttonPinR, HIGH);
  digitalWrite(buttonPinL, HIGH);  
  
  // setup outputs
  pinMode(RGBPin, OUTPUT);  
  pinMode(speakerPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  
  // initial RGB states
  digitalWrite(RGBPin, LOW);
 
  if(THAT_ADDRESS > 0){
	//Serial.println("WIRE BEGIN");
	Wire.begin(ME);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent); // register event
  }

}

void RGB( int pixel, uint32_t Color){
	RGB(pixel, c);
}

uint32_t Color(byte red, byte green, byte blue)
{
  uint32_t c;
  c = red;
  c <<= 8;
  c |= green;
  c <<= 8;
  c |= blue;
  return c;
}

void RGB(byte red, byte green, byte blue){
	analogWrite(RGBPin, c);
}


void RGB(int color)
{
  int redIntensity;
  int greenIntensity;
  int blueIntensity;

  if (color <= 255)          // zone 1
  {
    redIntensity = 255 - color;    // red goes from on to off
    greenIntensity = color;        // green goes from off to on
    blueIntensity = 0;             // blue is always off
  }
  else if (color <= 511)     // zone 2
  {
    redIntensity = 0;                     // red is always off
    greenIntensity = 255 - (color - 256); // green on to off
    blueIntensity = (color - 256);        // blue off to on
  }
  else // color >= 512       // zone 3
  {
    redIntensity = (color - 512);         // red off to on
    greenIntensity = 0;                   // green is always off
    blueIntensity = 255 - (color - 512);  // blue on to off
  }

  RGB(pixel, redIntensity, greenIntensity, blueIntensity);
}


void RGB(byte ADDRESS, int pixel, uint32_t c){
	if (ADDRESS == THIS_ADDRESS){
		RGB(pixel, uint32_t c);
	}
    else {
		#if !(defined(NOWIRE))
		Wire.beginTransmission(THAT_ADDRESS);
		Wire.write(2);     // instuction
		Wire.write(RGB(pixel, uint32_t c)); // led
		Wire.endTransmission();  
		#endif
	}
}
void RGB(byte ADDRESS, int color){
	if (ADDRESS == THIS_ADDRESS){
		RGB(color);
	}
    else {
		if(THAT_ADDRESS > 0){
			Wire.beginTransmission(THAT_ADDRESS);
			Wire.write(2);    		      // instuction
			Wire.write(highByte(color));  // led	
			Wire.write(lowByte(color));
			Wire.endTransmission();  
		}
	}
}

void motor(byte speed){
  analogWrite(motorPin, speed);
}


void motor(byte ADDRESS, byte speed){
	if (ADDRESS == THIS_ADDRESS){
		analogWrite(motorPin, speed);
	}
	else{
		if(THAT_ADDRESS > 0){
			Wire.beginTransmission(THAT_ADDRESS);
			Wire.write(4);      // instuction
			Wire.write(speed);  // led	
			Wire.endTransmission();  
		}
	}
}


int trim(){
  int val = analogRead(trimPin);
  return val;
}


int trim(byte ADDRESS){
	if (ADDRESS == THIS_ADDRESS){
		return trim();
	}
	else{
		return getReading(ADDRESS, 0);
	}
}


int getReading(byte ADDRESS, byte sensor){
	int delayTime = 1; // just a pause to let wire events happen

	if(THAT_ADDRESS > 0){
		// set pointer
		Wire.beginTransmission(THAT_ADDRESS);
		Wire.write(sensor);    // instuction
		Wire.endTransmission();
		delay(delayTime);
		
		// retrieve bytes
		Wire.requestFrom(int(ADDRESS), 1);
		MSB = Wire.read();
		delay(delayTime);
		Wire.requestFrom(int(ADDRESS), 1);
		LSB =Wire.read();
		delay(delayTime);
		unsigned int ret = ((unsigned int) MSB << 8) | LSB;
		return ret;
	}
}


int light(){
  int val = analogRead(lightPin);
  return 1023 - val;  
}


int light(byte ADDRESS){
	if (ADDRESS == THIS_ADDRESS){
		return light();
	}
	else{
		return getReading(ADDRESS, 2);
	}
}


float temp(){
  int val = analogRead(tempPin);
  float voltage = val * (5000 / 1024.0);    // voltage at pin milliVolts
  float degreesC = (voltage / 10);  // voltage to C
  return degreesC;
}


float temp(byte ADDRESS){
	int val;
	if (ADDRESS == THIS_ADDRESS){
		return temp();
	}
	else{
		val = getReading(ADDRESS, 4);
	}
	float voltage = val * (5000 / 1024.0);    // voltage at pin milliVolts
    float degreesC = (voltage / 10) - 50;  // voltage to C
	return degreesC;
}


int analog(){
  int val = analogRead(analogPin);
  return val;  
}


int analog(byte ADDRESS){
	if (ADDRESS == THIS_ADDRESS){
		return analog();
	}
	else{
		return getReading(ADDRESS, 6);
	}
}


boolean touch(){
  int val = analogRead(touchPin);
  if (val < 1000){
	return true;
  }
  else {
    return false;
  }
}


boolean touch(byte ADDRESS){
	if (ADDRESS == THIS_ADDRESS){
		return touch();
	}
	else{
		int val = getReading(ADDRESS, 6);
		if (val < 1000){
			return true;
		}
		else {
			return false;
		}
	}
}


boolean button(boolean side){ // return 1 if pressed, 0 if not
  // false == left, true == right
  if (side == LEFT){
    return !digitalRead(buttonPinL);
  }
  else {
    return !digitalRead(buttonPinR);
  }
}


boolean button(byte ADDRESS, boolean side){ // return 1 if pressed, 0 if not
	// false == left, true == right
	if (ADDRESS == THIS_ADDRESS){
		return button(side);
	}
	else{
		int val = getReading(ADDRESS, 8);
		if (side == LEFT){
			return !highByte(val);
		}
		else {
			return !lowByte(val);
		}
	}
}


void speaker(unsigned int frequency, unsigned long duration){
	tone(speakerPin, frequency, duration);
}


void speaker(byte ADDRESS, unsigned int frequency, unsigned long duration){
	byte buf[4];
	
	if (ADDRESS == THIS_ADDRESS){
		tone(speakerPin, frequency, duration);
	}
	else{
		// convert from an unsigned long int to a 4-byte array
		buf[0] = (int)((duration >> 24) & 0xFF) ;
		buf[1] = (int)((duration >> 16) & 0xFF) ;
		buf[2] = (int)((duration >> 8) & 0XFF);
		buf[3] = (int)((duration & 0XFF));
		if(THAT_ADDRESS > 0){
			Wire.beginTransmission(THAT_ADDRESS);
			Wire.write(3);    // instuction
			Wire.write(highByte(frequency));  // led
			Wire.write(lowByte(frequency));// state
			Wire.write(buf[0]);
			Wire.write(buf[1]);
			Wire.write(buf[2]);
			Wire.write(buf[3]);
			Wire.endTransmission(); 
		}
	}
}


void LED(byte led, boolean state){
	if (led < 8){
		// Mask desired bit on to global _data variable
		bitWrite(data, led, state);

		// Now we'll actually send that data to the shift register.
		shiftOut(datapin, clockpin, MSBFIRST, data);

		// Toggle the state of the latchPin,
		digitalWrite(latchpin, HIGH);
		digitalWrite(latchpin, LOW);
	}
	else if (led < 16){
		if (false){
			Serial.print("Send");	
			Serial.print(led);
			Serial.print(" ");
			Serial.println(state);
		}	
		if(THAT_ADDRESS > 0){
			Wire.beginTransmission(THAT_ADDRESS);
			Wire.write(1);    // instuction
			Wire.write(15 - led);  // led
			Wire.write(state);// state
			Wire.endTransmission(); 
		}
  }
}


void receiveEvent(int howMany){
	if(DEBUG){
		Serial.print("\nReceive Event: howMany = ");
		Serial.println(howMany);
	}

	// get data
	byte buf[8];
	int counter = 0;
	for (int x=0; x<howMany; x++){
		buf[x] = Wire.read();
	}

	// show data
	if(DEBUG){
		for(int x=0; x<howMany; x++){
			Serial.print(buf[x], DEC);
			Serial.print(", ");
		}
		Serial.println();
	}

	byte instruction = buf[0];

	// POINTER INSTRUCTION?
	if (howMany == 1){
		pointer = instruction;
		if(false){
			Serial.print("pointer=");
			Serial.println(pointer);
		}
	}	
	
	// LED INSTRUCTION
	else if (instruction == 1 && howMany == 3){
		if(DEBUG){Serial.println("LED command");}
		LED(buf[1], buf[2]);
	}	
	
	// RGB INSTRUCTION
	else if (instruction == 2){
		if(DEBUG){Serial.println("RGB command");}
		if (howMany == 4){
			byte RGBcolor[3];
			RGBcolor[0] = buf[1];
			RGBcolor[1] = buf[2];
			RGBcolor[2] = buf[3];
			RGB(RGBcolor);
		}
		else if (howMany == 3){
			RGB(word(buf[1], buf[2]));
		}
	}
	
	else if (instruction == 3 && howMany == 7){
		unsigned int frequency;
		unsigned long duration;	
		frequency = word(buf[1], buf[2]);
		duration  = ((unsigned long) buf[3]) << 24;
		duration |= ((unsigned long) buf[4]) << 16;
		duration |= ((unsigned long) buf[5]) << 8;
		duration |= ((unsigned long) buf[6]);
		speaker(frequency, duration);
	}
	
	else if (instruction == 4 && howMany == 2){
		byte speed = buf[1];
		analogWrite(motorPin, speed);
	}	
	
}


void requestEvent(){ // get analog reading and return MSB, LSB
	int val;
	if(DEBUG){
		Serial.print("p=");
		Serial.println(pointer);}
	if(pointer == 0){				// POT
		val = analogRead(trimPin);
	}
	else if (pointer == 2){			// LIGHT
		val = analogRead(lightPin);
	}
	else if (pointer == 4){			// TEMP
		val = analogRead(tempPin);
	}
	else if (pointer == 6){			// ANALOG
		val = analogRead(analogPin);
	}
	else if (pointer == 8){			// Buttons
		byte valL = digitalRead(buttonPinL);
		byte valR = digitalRead(buttonPinR);
		val = word(valL, valR);
	}	
	
	if(pointer % 2){ 				// Odd pointer, return LSB
		Wire.write(LSB);
	}
	else{
		MSB = highByte(val);
		LSB = lowByte(val);
		Wire.write(MSB);
		pointer++;	
	}
}

