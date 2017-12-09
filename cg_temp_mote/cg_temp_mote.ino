
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "f9ab06f97c4a45c7b086b3ee0e7d2622";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ABCD";
char pass[] = "******";
float h;
float t;
String IFTTT_KEY = "iYiYhj3KyPFEwyVRuJzEb"; 
// my api key  iYiYhj3KyPFEwyVRuJzEb(for testing)
const char* eventName  = "tempAlert";
const char* IFTTT_URL = "maker.ifttt.com";

#define DHTPIN 2          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
WidgetRTC rtc;


int lowLim;
int upLim;
int hours;
unsigned long currentMillis;
unsigned long previousMillis;
unsigned long interval = 300000;
int temp;
int hum;

BLYNK_WRITE(V7)
{
  lowLim = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V7 Slider value is: ");
  Serial.println(lowLim);
}

BLYNK_WRITE(V8)
{
  upLim = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V8 Slider value is: ");
  Serial.println(upLim);
}

void sendSensor()
{
   String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
   dht.begin();

  

 
 h = dht.readHumidity();
  t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  
 
 Serial.print("Temperature: ");
 Serial.println(t);
 Serial.print(temp);
  Serial.print("      ");
  Serial.println(hum);
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

 
  Serial.println();
  hours = String(hour()).toInt();
  Serial.print("hours :");
  Serial.println(hours);
  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
   Blynk.virtualWrite(V0, t, h);
   Serial.print("data sent");
 
 // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details
    
     
}


void setup()
{//dht.begin();
  // Debug console
  Serial.begin(9600);
  
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);

 // dht.begin();
  rtc.begin();
  dht.begin();
  Serial.print("Temperature: ");
  // Display digital clock every 10 seconds
  //timer.setInterval(30000L, clockDisplay);
  // Setup a function to be called every second
  timer.setInterval(60000L, sendSensor);
}

void loop()
{
  Blynk.run();
//  dht.begin();
  timer.run();
  // float hum = dht.readHumidity();
  //float temp = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  currentMillis = millis();
  if(t<=lowLim && t!=0) 
    {Serial.print("Low Lim: ");
    Serial.println(lowLim);
    Serial.print("Temperature: ");
    Serial.println(t);
      if((hours >= 19) && (hours <= 24))
        {
      if((currentMillis - previousMillis) > interval)
       {
        // Define the WiFi Client
       WiFiClient client;
       // Set the http Port
       const int httpPort = 80;

       // Make sure we can connect
       if (!client.connect(IFTTT_URL, httpPort))
          {
            return;
          }

       // We now create a URI for the request
       // String url = "/trigger/" + String(eventName) + "-"+String(eventCount)+ "/with/key/" + String(IFTTT_KEY);
       String url = "/trigger/" + String(eventName) + "/with/key/" + String(IFTTT_KEY);

       // Set some values for the JSON data depending on which event has been triggered
       IPAddress ip = WiFi.localIP();
       String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
       String value_1 = String(t);
       String value_2 = String(h);
       String value_3 = "";
       


        // Build JSON data string
        String data = "";
        data = data + "\n" + "{\"value1\":\"" + value_1 + "\"}"; //,"value2\":\""+ value_2 +"\"}";

        // Post the button press to IFTTT
        if (client.connect(IFTTT_URL, httpPort))
           {

             // Sent HTTP POST Request with JSON data
             client.println("POST " + url + " HTTP/1.1");
             Serial.println("POST " + url + " HTTP/1.1");
             client.println("Host: " + String(IFTTT_URL));
             Serial.println("Host: " + String(IFTTT_URL));
             client.println("User-Agent: Arduino/1.0");
             Serial.println("User-Agent: Arduino/1.0");
             client.print("Accept: *");
             Serial.print("Accept: *");
             client.print("/");
             Serial.print("/");
             client.println("*");
             Serial.println("*");
             client.print("Content-Length: ");
             Serial.print("Content-Length: ");
             client.println(data.length());
             Serial.println(data.length());
             client.println("Content-Type: application/json");
             Serial.println("Content-Type: application/json");
             client.println("Connection: close");
             Serial.println("Connection: close");
             client.println();
             Serial.println();
             client.println(data);
             Serial.println(data);

             previousMillis = currentMillis;
           }
      }
    }
    }
}
