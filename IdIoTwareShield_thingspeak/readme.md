###**Log the Temperature and Light values at 5 minutes interval using the IdIotWare Arduino Shield and ESP8266**

 This example shows how to log data from Arduino to Thingspeak using ESP8266   wifi module.
 ThingSpeak is an open source Internet of Things (IoT) application and API to store and 
 retrieve data from things using the HTTP protocol over the Internet or via a Local Area Network.
 ThingSpeak enables the creation of sensor logging applications, location tracking applications,
 and a social network of things with status updates.
 
 To log the data to thingspeak, you need to create thingspeak account and then create channel and 
 use that channel's API Key in this code.
 
 In this example we have used ELClient.h library to log data to thingspeak. Temperature values from
 LM35 temperature sensor and Light values from LDR are logged to thingspeak every 5  minutes for 24 hours. 
 This interval rate and duration is configurable. thingspeak needs 15 sec delay between updates.
 
