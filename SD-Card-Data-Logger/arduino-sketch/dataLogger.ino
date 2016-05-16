/*
  SD card temperature logger
 This example shows how to log data from LM35 temperature sensor
 to an SD card using the SD library.
 In this example we have created a CSV file with the temperature values
 every one minute for 15 minutes. This interval rate and duration is configurable.
 If there is existing file, then it will create new file with different name.
 
  
 The circuit:
 * LM35 sensor on analog pin A3
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 */

#include <SPI.h>
#include <SD.h>

File dataFile;   // the logging file
char filename[] = "LOGGER00.CSV";
const int chipSelect = 4;   //cs pin of SD card shield
int tempPin = A3;      // LM 35 is connected to A3 pin.
int temperature;
unsigned long time=0;

void setup() 
    {
      // Open serial communications and wait for port to open:
      Serial.begin(9600);
      while (!Serial)
          {
            ; // wait for serial port to connect. Needed for native USB port only
          }

      Serial.print("Initializing SD card...");

     // see if the card is present and can be initialized:
     if (!SD.begin(chipSelect)) 
        {
          Serial.println("Card failed, or not present");
          // don't do anything more:
          return;
        }
     Serial.println("card initialized.");

     // create a new file
     //char filename[] = "LOGGER00.CSV";
     for (uint8_t i = 0; i < 100; i++) 
        {
          filename[6] = i/10 + '0';
          filename[7] = i%10 + '0';
          if (! SD.exists(filename)) 
            {
              // only open a new file if it doesn't exist
              dataFile = SD.open(filename, FILE_WRITE);
              break;  // leave the loop!
            }
        }

      if (! dataFile) 
        {
          Serial.println("couldnt create file");
        }

     Serial.print("Logging to: ");
     Serial.println(filename);
    
   }

void loop() 
    {
     dataSamples();    // here we are logging data at interval of 1 minute for 15 mintutes, i.e, 15 samples.
                            // if you want to save data for 2 hours then simply multiply 2 by 60 which will give 
                            // you value of 120 minutes then use 120 as second parameter.
    }    
// this method will log data to SD card at particular interval and for paricular duration
int dataSamples()
   { 
     // here we are logging data at interval of 1 minute for 15 mintutes, i.e, 15 samples.
     // if you want to save data for 2 hours then simply multiply 2 by 60 which will give 
     // you value of 120 minutes then change the varible duration to 120. 
     int samplingTime = 3;  //this variable is interval at which you want to log the data to SD card.
     int duration = 30;     //this variable is duration which is the total time for which you want to log data.

      temperature = ( 5.0 * analogRead(tempPin) * 100.0) / 1024.0;
      unsigned long  elapsedTime = millis()/1000;   // this variable will keep track of elapsed time
      while(((millis()/1000)-elapsedTime) < 60);    // this loop will do nothing until a minute has passed 
      time++;                                       //increment time after each minute
 
      if(time >=720)   //after 12 hours i.e, 720 minutes time will reset to 0
      time = 0;   
      if ((duration >= time) && (time % samplingTime == 0))
          {
            LogToSDcard();  //Log to SD using commands under void LogToSD()  
            // print to the serial port too: 
            Serial.println(time);              
            Serial.print("Temperature: ");
            Serial.println(temperature);    
         }
           
   }

void LogToSDcard()
   {
     // open the file. note that only one file can be open at a time,
     // so you have to close this one before opening another.
     dataFile = SD.open(filename, FILE_WRITE);
     // if the file is available, write to it:
     if (dataFile) 
       { 
         dataFile.print(temperature);
         dataFile.print(",");
         dataFile.close();
       } 
  
      //if the file isn't open, pop up an error:
     else 
       {
        Serial.println("error opening datalog.txt");
        delay(2000);
       }
   }
