#**WiFi POSTBOX**
In this example we are using ESP8266 and IdIoTWare Shield and Arduino Board. LDR on IdIoTWare shield 
is used as sensor to detect letter in the letterbox. WS2812 Led (addressable RGB LED) 
on IdIoTWare shield is continuously ON (WHITE Color) and reflects light on LDR. As 
soon as new letter dropped in postbox light intensity on LDR changes due to reflection 
and event gets triggered.
Here we are using IFTTT to trigger an event.

IFTTT is a free web-based service that allows users to create chains of simple conditional statements,
called "recipes", which are triggered based on changes to other web services such as Gmail, Facebook,
Instagram, and many more.IFTTT is an abbreviation of "If This Then That"
Create account on IFTTT and create your recipe.
    
We are using Maker and Gmail channel to trigger an event.
    
If there is new letter in postbox, Arduino will send POST request to maker channel.  
if Maker then Gmail
If Maker Event "New Letter", then send an email from "abcd@gmail.com" 
