#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int pos = 0;    // variable to store the servo position 
int pwmValue = 1023;

//int motor_left[] = {14, 12};
//int motor_right[] = {13, 15};

// wifi connection variables
const char* ssid = "CuriosityGym-BCK";
const char* password = "#3Twinkle3#";
boolean wifiConnected = false;

// UDP variables
unsigned int localPort = 8888;
WiFiUDP UDP;
boolean udpConnected = false;
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
         
// Initialise wifi connection
    wifiConnected = connectWifi();

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

// if there’s data available, read a packet
            int packetSize = UDP.parsePacket();
            if (packetSize)
            {
                Serial.println("");
                Serial.print("Received packet of size ");
                Serial.println(packetSize);
                Serial.print("From ");
                IPAddress remote = UDP.remoteIP();
                for (int i =0; i < 4; i++)
                {
                    Serial.print(remote[i], DEC);
                    if (i < 3)
                    {
                        Serial.print(".");
                    }
                }
                Serial.print(", port ");
                Serial.println(UDP.remotePort());

// read the packet into packetBufffer
                UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
                Serial.println("Contents:");
                int fwdORbckwd = packetBuffer[0];//*256 + packetBuffer[1];
                Serial.print("fWd: ");
                Serial.println(fwdORbckwd);
                
                int leftORright = packetBuffer[1];//*256 + packetBuffer[1];
                Serial.print("bck: ");
                Serial.println(leftORright);
              
                int head = packetBuffer[3];//*256 + packetBuffer[1];
                Serial.print("head1: ");
                Serial.println(head);
                int turnHead = map(head, 0,255, 40,180);
                Serial.print("turn head: ");
                Serial.println(turnHead);
                myservo.write(turnHead);
                if(fwdORbckwd > 150)
                  {
                    int motorSpeed= map(fwdORbckwd,150,255,512,1023);
                    Serial.println("Move forward");
                    forward(motorSpeed);
                  }
                else if(fwdORbckwd < 105)
                  {
                    int motorSpeed= map(fwdORbckwd,105,0,1023,512);
                    Serial.println("Move backward");
                    backward(motorSpeed);
                  }
                else if(leftORright > 205)
                  {
                    Serial.println("Turn right");
                    right();
                  }
                else if(leftORright < 50)
                  {
                    Serial.println("Turn left");
                    left();
                  }  
                else
                  {
                    Serial.println("STOP!!"); 
                    stop();   
                  }

// send a reply, to the IP address and port that sent us the packet we received
                UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
                UDP.write(ReplyBuffer);
                UDP.endPacket();

// turn LED on or off depending on value recieved
                //digitalWrite(5,value);
            }
            delay(10);

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
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (i > 10) {
            state = false;
            break;
        }
        i++;
    }
    if (state) {
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
Serial.print("IP address: ");
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
 
void forward(int speed)
{
    analogWrite(5, speed);
    analogWrite(4, speed);
    digitalWrite(0, HIGH);
    digitalWrite(2, HIGH);
}
 
void backward(int speed)
{
    analogWrite(5, speed);
    analogWrite(4, speed);
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

