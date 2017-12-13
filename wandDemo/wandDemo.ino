#include <ESP8266WiFi.h>

const char* ssid     = "Pluto_225"; //Change this value for your network
const char* password = "dronapluto75"; //Change this value for your network
boolean connected = false;
boolean colorToggle = true;
String s;
void WiFiEvent(WiFiEvent_t event) {
    //Serial.printf("[WiFi-event] event: %d\n", event);  
    switch(event) {
    case 3:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connected = true;
        break;
    case 6:
        Serial.println("WiFi lost connection");
        connected = false;
        break;
    }

    // 0 : WL_IDLE_STATUS when Wi-Fi is in process of changing between statuses
    // 1 : WL_NO_SSID_AVAILin case configured SSID cannot be reached
    // 3 : WL_CONNECTED after successful connection is established
    // 4 : WL_CONNECT_FAILED if password is incorrect
    // 6 : WL_DISCONNECTED if module is not configured in station mode

}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
    Serial.begin(115200);
    WiFi.disconnect(true);
    delay(1000);
    WiFi.onEvent(WiFiEvent);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

}

// the loop function runs over and over again forever
void loop() {   
    
      sendPacket1();
 
}

void sendPacket1() {
    const uint16_t port = 23;
    const char * host = "192.168.4.1";  //Change to whatever your LED Controller IP is

     WiFiClient client;
    
     if (client.connect(host, port)) //Try to connect to TCP Server
     {
       char command[] = "\x24\x4d\x3c\x00\x6c\x6c";
       //Serial.print(command);
       client.write((uint8_t *)command, sizeof(command));
      // delay(3); 
     } 
     
     else
     {
        Serial.println("connection failed ... ");
     }

      while(client.available())
      {
        //Read from LED Controller
        int c = client.read();
        Serial.print(c,HEX);
        
      }
     Serial.println();
     // 
     for(int i=0; i<180; i++)
        {
          delay(5); 
        }
  /* 
   unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
        
      return;
    //}
  }*/
      if (!client.connected())
      {
        Serial.println();
        Serial.println("disconnecting ... ");
        client.stop();
        for(;;);
      }  
      yield();
     client.flush();
    
}
