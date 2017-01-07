#**Back Door Intrusion Detector**
   
   In this example we are using ESP8266 and IdIoTWare Shield and Arduino Board. Magnetic Reed Switch
is used to detect intrusion.                                           
Magnetic Reed Switch :                                                  
A magnetic contact switch is basically a reed switch encased in a plastic shell so that  you can easily          
apply them in a door, a window or a drawer to detect if the door is open or closed..

This example uses a  Magnetic Reed Switch The electrical circuit is closed when a magnet is near the             
switch (less than 13 mm (0.5’’) away). When the magnet is far away from the switch, the circuit is open.             
When it is open, a signal is  sent to  arduino uno that sends email to user using ESP8266.                         
Here we are using IFTTT to trigger an event.
                             
IFTTT is a free web-based service that allows users to create chains of simple conditional statements,                    
called "recipes", which are triggered based on changes to other web services such as Gmail, Facebook,                     
Instagram, and many more.IFTTT is an abbreviation of "If This Then That"                                     
Create account on IFTTT and create your recipe.                                       

We are using Maker and Gmail channel to trigger an event.                                         

If magnetic reed switch is open(if door is open), Arduino will send POST request to maker channel.                 
if there is any event on Maker channel  then send mail using Gmail                                           
If Maker Event "intrusion_detected", then send an email from "abcd@gmail.com"

