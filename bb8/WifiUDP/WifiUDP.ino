#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include <WiFiClient.h> 


// wifi connection variables
const char *ssid = "ESPap";
const char *password = "thereisnospoon";
boolean wifiConnected = true;

byte forward=0, left=0, head =0;
// UDP variables
unsigned int localPort = 8888;
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back
IPAddress ip(192, 168, 1,1); 
void setup() {
// Initialise Serial connection
    Serial.begin(115200);

// Initialise wifi connection
    //wifiConnected = connectWifi();
    //WiFi.config(ip);
    WiFi.softAP(ssid, password);

// only proceed if wifi connection successful
    if (wifiConnected) {
        udpConnected = connectUDP();
        if (udpConnected) {
// initialise pins
            pinMode(5,OUTPUT);
        }
    }
}

void loop() {
// check if the WiFi and UDP connections were successful
    if (wifiConnected) {
      //Serial.println("Wifi Connected");
        if (udpConnected) {
         // Serial.println("UDP Connected");

// if there’s data available, read a packet
            int packetSize = UDP.parsePacket();
            if (packetSize)
            {
                Serial.println("");
                Serial.print("Received packet of size ");
                Serial.println(packetSize);
                Serial.print("From ");
                IPAddress remote = UDP.remoteIP();
                for (int i= 0; i < 4; i++)
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
                //int value = packetBuffer[0]*256 + packetBuffer[1];
                forward=packetBuffer[0];
                left=packetBuffer[1];
                head=packetBuffer[3];
                Serial.print("Forward:");
                Serial.println(forward, DEC);

                Serial.print("Left:");
                Serial.println(left, DEC);

                Serial.print("Head");
                Serial.println(head, DEC);
                

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
