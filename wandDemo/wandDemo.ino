/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("Pluto_225", "dronapluto75");

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
}


void loop() {
    const uint16_t port = 23;
    const char * host = "192.168.4.1"; // ip or dns

    
    
   // Serial.print("connecting to ");
   // Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        //Serial.println("wait 5 sec...");
      //  delay(5000);
        return;
    }
   if (client.connect(host, port)) //Try to connect to TCP Server
     {
        client.flush();
       // This will send the request to the server
       char command[] = "\x24\x4d\x3c\x10\xc8\xdc\x05\xdc\x05\xeb\x03\xdc\x05\xb0\x04\xe8\x03\xdc\x05\xdc\x05\xb6\x24\x4d\x3c\x00\x6c\x6c";
       //Serial.print(command);
       client.write((uint8_t *)command, sizeof(command));
       //read back one line from server
     } 
   while(client.available())
      {
        //Read from LED Controller
        int c = client.read();
        Serial.print(c,HEX);   
        //String line = client.readStringUntil('\r');
        //client.println(line); 
      }
    

  //  Serial.println("closing connection"); 
   // Serial.println("wait 5 sec...");
   // delay(5000);
}
