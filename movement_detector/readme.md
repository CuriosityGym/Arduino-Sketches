#**Motion Detector**
    
In this example we are using ESP8266 and IdIoTWare Shield and Arduino Board. PIR sensor
is used to detect motion.
PIR Sensor :
  PIR sensors allow you to sense motion, almost always used to detect whether a human has moved in or out of the sensors range.
  They are often referred to as PIR, "Passive Infrared", "Pyroelectric", or "IR motion" sensors.

This example uses a motion-detecting sensor (PIR sensor)  which detects the infrared energy omitted from human's body. 
When PIR detects motion, the infrared energy detected by the sensor changes and it triggers the event and signal is  sent to 
arduino uno that sends email to user using ESP8266.
Here we are using IFTTT to trigger an event.

IFTTT is a free web-based service that allows users to create chains of simple conditional statements,
called "recipes", which are triggered based on changes to other web services such as Gmail, Facebook,
Instagram, and many more.IFTTT is an abbreviation of "If This Then That"
Create account on IFTTT and create your recipe.

We are using Maker and Gmail channel to trigger an event.

If there is any motion detected by PIR sensor, Arduino will send POST request to maker channel.  
if there is any event on Maker channel  then send mail using Gmail
If Maker Event "motion_detected", then send an email from "abcd@gmail.com" 
