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
    //if we are connected
    //if(connected){
     // digitalWrite(LED_BUILTIN, LOW); //Turn on LED on the Board
      sendPacket1();
   // }
  //  else
  //  {
  //    digitalWrite(LED_BUILTIN, HIGH); //Turn off LED on the Board
  //    setup();
  //  }
}

void sendPacket1() {
    const uint16_t port = 23;
    const char * host = "192.168.4.1";  //Change to whatever your LED Controller IP is

     WiFiClient client;
/*
     if (client.connect(host, port)) //Try to connect to TCP Server
     {
       char command[] = "\x24\x4d\x3c\x10\xc8\xdc\x05\xdc\x05\xeb\x03\xdc\x05\xb0\x04\xe8\x03\xdc\x05\xdc\x05\xb6\x24\x4d\x3c\x00\x6c\x6c";
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
      }delay(2);
    //  Serial.println();
*/
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
      //delay(500); 
   // client.flush();
}

/*
// https://github.com/CuriosityGym/Arduino-Sketches/tree/master/wandDemo


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
    Serial.println();

  //  Serial.println("closing connection");
   delay(2);
    
   // Serial.println("wait 5 sec...");
   // delay(5000);
}

*/
