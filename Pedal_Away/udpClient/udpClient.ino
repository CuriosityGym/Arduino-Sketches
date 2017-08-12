#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NeoPixelBus.h>
#include <EEPROM.h>

const uint16_t PixelCount = 144; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266


const char* ssid = "BicycleGenerator";
const char* password = "#3Twinkle3#";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back

NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart400KbpsMethod> strip(PixelCount, PixelPin);



int redColor = 0;
int greenColor = 0;
int blueColor = 0;
RgbColor black(0);
RgbColor green(0, 255, 0);
uint8_t brightness = 100;
int ledMeter;
int minSpeed = 0;
int maxSpeed = 70;

int highestSpeed = 45;
//String highSpeed= "";
boolean updateSpeed = false;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);  //Initialize EEPROM
  /*  for(int i = 0; i < 1; ++i)
       {
         highestSpeed = EEPROM.read(i);
       }
       Serial.begin(115200);
    Serial.print("Highest Speed: ");
    Serial.println(highestSpeed);
  */
  strip.Begin();
  strip.Show();
  Serial.println();
  WiFi.mode(WIFI_AP);
  // Serial.printf("Connecting to %s ", ssid);
  WiFi.softAP(ssid, password);
  /*while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.print(".");
      }
    Serial.println(" connected");*/

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  //Serial.println("IP address: ");
  //Serial.println(WiFi.softAPIP());




}


void loop()
{
  currentMillis = millis();
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    previousMillis = currentMillis;
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    //Serial.printf("UDP packet contents: %s\n", incomingPacket);
    int firstTestPacket = atoi(incomingPacket);
    if (firstTestPacket == 1234)
    {
      for (int i = 0; i < 2; i++)
      {
        for (int i = 0; i <= 144; i++)
        {
          strip.SetPixelColor(i, green);
          strip.Show();
          //  delay(10);
        }
        delay(1000);
        for (int i = 144; i >= 0; i--)
        {
          strip.SetPixelColor(i, black);
          strip.Show();
          //  delay(10);
        }
      }
    }
    else
    {
      int actualSpeed = atoi(incomingPacket);
      //Serial.print("HIghest Speed: ");
      //Serial.println(highestSpeed);
      ledMeter = actualSpeed;
      ledMeter = map(ledMeter, minSpeed, highestSpeed, 0, 144);
      //Serial.printf("cycle speed: %d\n ", ledMeter);
      if (actualSpeed > highestSpeed)
      {
        highestSpeed = actualSpeed;
        //highSpeed = String(highestSpeed);
        updateSpeed = true;
      }
      // send back a reply, to the IP address and port we got the packet from
      //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      //Udp.write(replyPacekt);
      //Udp.endPacket();
    }
  }

  for (int i = 0; i < ledMeter; i++)
  {
    if (i < 48)
    {
      redColor = 0;
      blueColor = 0;
      greenColor = 0;
      redColor = map(i, 0, 48, brightness, 0);
      greenColor = map(i, 0, 48, 0, brightness);

    }
    if ((i > 48 ) && (i < 96))
    {
      redColor = 0;
      blueColor = 0;
      greenColor = 0;
      greenColor = map(i, 49, 96, brightness, 0);
      blueColor = map(i, 49, 96, 0, brightness);

    }
    if ((i > 96 ) && (i <= 144))
    {
      redColor = 0;
      blueColor = 0;
      greenColor = 0;
      blueColor = map(i, 97, 144, brightness, 0);
      redColor = map(i, 97, 144, 0, brightness);

    }
    strip.SetPixelColor(i, RgbColor(redColor, greenColor, blueColor));

    //strip.SetPixelColor(i, blue);
    //strip.Show();
    //  delay(10);
  }
  //   strip.Show();
  for (int i = ledMeter; i <= 144; i++)
  {
    strip.SetPixelColor(i, black);

    //  delay(10);
  }
  strip.Show();
  if ((currentMillis - previousMillis) > 10000 && updateSpeed == true)
  {

    for (int i = 0; i < 512; i++)
      EEPROM.write(i, 0);
    // Serial.println("Writing HIghest Spped to EEPROM...");

    for (int i = 0; i < 1; ++i)
    {
      EEPROM.write(i, highestSpeed);
    }
    updateSpeed = false;
    //after update
    for (int i = 0; i < 1; ++i)
    {
      highestSpeed = EEPROM.read(i);
    }


    //Serial.print("Highest Speed>>>>: ");
    //Serial.println(highestSpeed);

  }
}
