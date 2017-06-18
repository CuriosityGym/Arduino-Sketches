#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <Servo.h>
#define MOVES 30
#define OPPMOVES 15
#define INTERVAL 3000

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
unsigned long elapsedTime = millis();
int pos = 0;    // variable to store the servo position
int pwmValue = 1023;
int delayValRev = 50;
int delayValAct = delayValRev * 2 / 3;
byte oppmov;
//int motor_left[] = {14, 12};
//int motor_right[] = {13, 15};

// wifi connection variables
const char* ssid = "CGBotSmartCar";
const char* password = "#3Twinkle3#";
boolean wifiConnected = true;

// UDP variables
unsigned int localPort = 8888;
WiFiUDP UDP;
boolean udpConnected = false;
byte forwardMoves = 0;
byte backwardMoves = 0;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);
  myservo.attach(14);  // attaches the servo on pin 9 to the servo object

  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  delay(1000);
  // Initialise wifi connection

  WiFi.softAP(ssid, password);
  // only proceed if wifi connection successful
  if (wifiConnected) {
    udpConnected = connectUDP();
    if (udpConnected) {
      // initialise pins
      // pinMode(5,OUTPUT);
    }
  }


}

void loop() {


  // check if the WiFi and UDP connections were successful
  if (wifiConnected) {
    if (udpConnected) {
      //Serial.println((millis() - elapsedTime));
      if ((millis() - elapsedTime) > INTERVAL) //Send a Byte after "Interval" time
      {
        elapsedTime = millis();
        udpSend(12.23);
      }

      // if there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      if (packetSize)
      {

        // read the packet into packetBufffer
        UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents:");
        int fwdORbckwd = packetBuffer[0];//*256 + packetBuffer[1];
        //Serial.print("fWd: ");
        Serial.println(fwdORbckwd);

        int leftORright = packetBuffer[1];//*256 + packetBuffer[1];
        //Serial.print("bck: ");
        Serial.println(leftORright);

        int head = packetBuffer[3];//*256 + packetBuffer[1];
        //Serial.print("head1: ");
        Serial.println(head);
        int turnHead = map(head, 0, 255, 40, 180);
        //Serial.print("turn head: ");
        Serial.println(turnHead);
        myservo.write(turnHead);
        if (fwdORbckwd > 205)
        {
          Serial.println("Move forward");
          forward();
          delay(delayValRev);


        }
        else if (fwdORbckwd < 50)
        {
          Serial.println("Move backward");
          backward();
          delay(delayValRev);

        }

        if (leftORright > 205)
        {
          Serial.println("Turn right");
          left();
          delay(delayValRev);
        }
        else if (leftORright < 50)
        {
          Serial.println("Turn left");
          right();
          delay(delayValRev);
        }
      }
      else //if no packet Recieved, stop
      {

        stop();

      }

    }

  }

}

// connect to UDP – returns true if successful or false if not
boolean connectUDP() {
  boolean state = false;

  Serial.println("");
  Serial.println("Connecting to UDP");

  if (UDP.begin(localPort) == 1) {
    Serial.println("Connection successful");
    state = true;
  }
  else {
    Serial.println("Connection failed");
  }

  return state;
}
// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  //Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    //Serial.print("Connected to ");
    //Serial.println(ssid);
    //Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;

}

// --------------------------------------------------------------------------- Drive

void stop(void)
{
  analogWrite(5, 0);
  analogWrite(4, 0);
}
void softStart()
{
  analogWrite(5, pwmValue);
  analogWrite(4, pwmValue);
}

void forward(void)
{
  analogWrite(5, pwmValue);
  analogWrite(4, pwmValue);
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
}

void backward(void)
{
  analogWrite(5, pwmValue);
  analogWrite(4, pwmValue);
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
}

void left(void)
{
  analogWrite(5, pwmValue);
  analogWrite(4, pwmValue);
  digitalWrite(0, LOW);
  digitalWrite(2, HIGH);
}

void right(void)
{
  analogWrite(5, pwmValue);
  analogWrite(4, pwmValue);
  digitalWrite(0, HIGH);
  digitalWrite(2, LOW);
}

unsigned char udpSend( float msg)
{
  UDP.beginPacket("192.168.4.2", 1111);
  Serial.println( sizeof(msg));
  //Serial.println();
  //UDP.write(msg, sizeof(msg));
  float iVarToCast = 123.456;
  char buffer1[7];
  dtostrf(iVarToCast, 7, 3, buffer1);
  UDP.write(buffer1, sizeof(buffer1));
//  dtostrf()
  return UDP.endPacket();
}

