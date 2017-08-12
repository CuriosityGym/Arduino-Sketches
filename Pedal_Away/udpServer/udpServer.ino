/*
   This sketch sends broadcast udp message.

   You need to get streamId and privateKey at data.sparkfun.com and paste them
   below. Or just customize this script to talk to other HTTP servers.

*/

#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <EEPROM.h>
#define TACHOINPUT 5
#define FACTOR 1.6*1000*3600
unsigned long int lastMicros = 0;

unsigned int speedKMPH = 0;
const char* ssid     = "BicycleGenerator";
const char* password = "#3Twinkle3#";
IPAddress ipBroadCast(192, 168, 4, 1);
unsigned int udpRemotePort = 4210;
unsigned int udplocalPort = 2390;
const int UDP_PACKET_SIZE = 48;
char udpBuffer[ UDP_PACKET_SIZE];
WiFiUDP udp;
//================================================================
// Setup hardware, serial port, and connect to wifi.
//================================================================

// unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
//char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back

boolean sent = false;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
boolean sendOnce = false;

void setup()
{
  Serial.begin(115200);
  //delay(10);
  // put your setup code here, to run once:
  pinMode(TACHOINPUT, INPUT);
  attachInterrupt(TACHOINPUT, tachoInterrupted, RISING);
  Serial.begin(115200);
  lastMicros = micros();
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //  Try to connect to wifi access point
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");

  // set udp port for listen
  udp.begin(udplocalPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  String value = "1234";
  value.toCharArray(udpBuffer, UDP_PACKET_SIZE);
  udp.beginPacket(ipBroadCast, udpRemotePort);
  udp.write(udpBuffer, sizeof(udpBuffer));
  udp.endPacket();

}

//================================================================
// Function to send udp message
//================================================================
void fncUdpSend()
{
  String value = String(speedKMPH);
  value.toCharArray(udpBuffer, UDP_PACKET_SIZE);
  //Serial.println(udpBuffer);
  udp.beginPacket(ipBroadCast, udpRemotePort);
  udp.write(udpBuffer, sizeof(udpBuffer));
  udp.endPacket();
}

//================================================================
// LOOP MAIN
//================================================================
// send udp packet each 5 secconds

void loop()
{
  currentMillis = millis();
  //Serial.print("currentMillis:  ");
  //Serial.println(currentMillis);
  if ( sent == true)
  {
    previousMillis = currentMillis;
    sent = false;
    sendOnce = true;
  }
  if ((currentMillis - previousMillis) > 5000 && sendOnce == true)
  {
    speedKMPH = 0;
    fncUdpSend();
    sendOnce = false;
  }


  // fncUdpSend();
  // print the received signal strength:
  // long rssi = WiFi.RSSI();
  //Serial.print("signal strength (RSSI):");
  //Serial.print(rssi);
  // Serial.println(" dBm");
  // delay(250);*/
}

void tachoInterrupted()
{
  unsigned long int duration = micros() - lastMicros;
  lastMicros = micros();
  if (duration > 1000)
  {
    speedKMPH = FACTOR / duration;
  }
  // Serial.println(speedKMPH);

  if (speedKMPH <= 75)
  {
    fncUdpSend();
    sent = true;
    // Serial.println("SENT");
  }
}

