/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include <NeoPixelBus.h>

const uint16_t PixelCount = 24; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);

const char* ssid = "New Curiosity Gym";
const char* password = "CuriosityTwinkle";

const char* host = "uberrides.herokuapp.com";
const char* resource1 = "/hasTokenExpired";
const char* resource2 = "";
const char* resource3 = "/bookUber";
const char* resource4 = "/viewRideStatus";
const int httpsPort = 443;
String line="";
String json="";
// Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  
// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";
int pushButton = 0;
int buttonState=0;
boolean checkStatus = false;
boolean rideIn_progress = false;
const unsigned long BAUD_RATE = 9600;                 // serial connection speed
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response

// The type of data that we want to extract from the page
struct UserData {
  char name[32];
  char company[32];
};
UserData userData;
String reply1="processing";
String reply2="accepted";
String reply3="in_progress";
RgbColor red(255,0, 0);
RgbColor green(0,255,0);
RgbColor blue(0,0, 255);
RgbColor yellow(255,255, 0);
RgbColor cyan(0,255,255);
RgbColor pink(255,0, 255);
RgbColor black(0,0, 0);
void setup() {
  pinMode(pushButton, INPUT);
  strip.Begin();
    strip.Show();
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  strip.SetPixelColor(1,RgbColor(red));
  strip.SetPixelColor(2,RgbColor(red));
  strip.Show();
  strip.SetPixelColor(1,RgbColor(black));
  strip.SetPixelColor(2,RgbColor(black));
  strip.Show();
  
}

void loop() {
  buttonState = digitalRead(pushButton);
  // print out the state of the button:
  //Serial.println(buttonState);
  if((buttonState == 0) && checkStatus == false) 
    {
      sendRequest(resource1);
      Serial.println(line);
      Serial.println("loop");
      if(line == "Yes")
         {
          strip.SetPixelColor(1,RgbColor(red));
          strip.SetPixelColor(6,RgbColor(red));
          strip.SetPixelColor(12,RgbColor(red));
          strip.SetPixelColor(18,RgbColor(red));
          strip.Show();
          Serial.println("Token Expired, Generate new Token");
          sendRequest(resource2);
        //  delay(1000);
          if(line == "OK")
            {
              Serial.println("New Token Generated");
              sendRequest(resource1);
              strip.SetPixelColor(1,RgbColor(green));
              strip.SetPixelColor(6,RgbColor(green));
              strip.SetPixelColor(12,RgbColor(green));
              strip.SetPixelColor(18,RgbColor(green));
              strip.Show();
              delay(1000);
              
            }
         }
      if(line == "No")
        {
          Serial.println("Book Uber");
          sendRequest(resource3);
          if (readReponseContent(&userData)) 
             {
               printUserData(&userData);
               if(userData.name == "processing");
                 {
                   checkStatus = true;
                 }
             }
        }   
                   
    }
  if(checkStatus == true)
    {
      Serial.println("check status");
      sendRequest(resource4);
     
      if (rideIn_progress == false)
         {
         if(readReponseContent(&userData)) 
             {
               
               //String response((char*)userData.name);
              //  Serial.print("response:  ");
                // Serial.println(response);
               //  strcpy(x,userData->name);
               if(printUserData(&userData) == "processing" )
                 {
                   Serial.println("Accept the Ride");
                   for(int i=0; i<PixelCount; i++)
                      { 
                        strip.SetPixelColor(i,RgbColor(red));
                        delay(10);
                      }
                   strip.Show();   
                 }
               if(printUserData(&userData) == "accepted" )
                 {
                   Serial.println("Lets Go");
                   for(int i=0; i<PixelCount; i++)
                      { 
                        strip.SetPixelColor(i,RgbColor(yellow));
                        delay(10);
                      }
                   strip.Show(); 
                 }
               if(printUserData(&userData) == "in_progress")
                 {
                   Serial.println("enjoy the Ride");
                   for(int i=0; i<PixelCount; i++)
                      { 
                        strip.SetPixelColor(i,RgbColor(blue));
                        delay(10);
                      }
                   strip.Show(); 
                   rideIn_progress = true;  
                 }
             } 
         }     
      if(rideIn_progress == true)
         {
          Serial.println(line.substring(69,101));
           if(line.substring(69,101) == "User is not currently on a trip.") 
              {
                Serial.println("Ride completed");
                Serial.println(line);
                rideIn_progress = false;      
                checkStatus = false;
                for(int i=0; i<PixelCount; i++)
                      { 
                        strip.SetPixelColor(i,RgbColor(green));
                        delay(10);
                      }
                   strip.Show(); 
              }
         }  
    }
}

void sendRequest(const char* resource)
{
      Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  //String url = "/bookUber";
  Serial.print("requesting URL: ");
  Serial.println(resource);

  client.print(String("GET ") + resource + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
     line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  json = line;
     }
     
bool readReponseContent(struct UserData* userData) {
  // Compute optimal size of the JSON buffer according to what we need to parse.
  // See https://bblanchon.github.io/ArduinoJson/assistant/
  const size_t BUFFER_SIZE =
      JSON_OBJECT_SIZE(8)    // the root object has 8 elements
      + JSON_OBJECT_SIZE(5)  // the "address" object has 5 elements
      + JSON_OBJECT_SIZE(2)  // the "geo" object has 2 elements
      + JSON_OBJECT_SIZE(3)  // the "company" object has 3 elements
      + MAX_CONTENT_SIZE;    // additional space for strings

  // Allocate a temporary memory pool
  DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);

  JsonObject& root = jsonBuffer.parseObject(json.c_str());

  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }

  // Here were copy the strings we're interested in
  //if(!rideIn_progress)
  strcpy(userData->name, root["status"]);
  //if(rideIn_progress)
   //  strcpy(userData->company, root["errors"]["status"]);
  // It's not mandatory to make a copy, you could just use the pointers
  // Since, they are pointing inside the "content" buffer, so you need to make
  // sure it's still in memory when you read the string

  return true;
}

// Print the data extracted from the JSON
String printUserData(const struct UserData* userData) {
  Serial.print("STATUS = ");
  Serial.println(userData->name);
  String response((char*)userData->name);
  
  return response;
}

// Close the connection with the HTTP server
void disconnect() {
  Serial.println("Disconnect");
  client.stop();
}

// Pause for a 1 minute
void wait() {
  Serial.println("Wait 3 seconds");
  delay(53000);
}     
