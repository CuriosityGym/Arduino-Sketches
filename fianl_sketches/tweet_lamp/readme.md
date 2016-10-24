# **idIoTWare Shield Twitter Lamp**                                    
##### **Attention: Please install all libraries from our Github Repository to enable this example to run.**
    
In this example we are using ESP8266-01 Wifi Module  and onboard WS2812 multicolor led.

In this example we are using Blynk HTTP RESTful API. 
Blynk HTTP RESTful API allows to easily read and write values to/from Pins in Blynk apps and 
Hardware (microcontrollers and microcomputers like Arduino, Raspberry Pi, ESP8266, Particle, etc.).
Every PUT request will update Pin's state both in apps and on the hardware. Every GET request will 
return current state/value on the given Pin. We also provide simplified API so you can do updates via GET requests.
You can read on how to do it here http://docs.blynkapi.apiary.io/#reference/0/get-pin-value/write-pin-value-via-get?console=1
Use your Auth Token number in url to get data from server.

In this example we have used ESP8266_Lib.h and BlynkSimpleShieldEsp8266.h  library to make communication between arduino
and Blynk HTTP RESTful API. We have used Heroku Deployment Application to receive tweet from our twitter handle and then this application sends get request to Blynk HTTP RESTful API.  

#####**Setup:**                                                          

1.install all libraries from our Github Repository to enable this example to run.                                   
2.create account on BLYNK and use your Auth Token number in code and url.                                                      

#####**Precautions:**

1.Enter correct Auth Token number.                                                                                        
2.Virtual pin number in code and url must be same.                                                                            
3.Check jumper for WS2812 multicolor led is connected properly.                                                 

#####**Images:** put link here
#####**Video:**  put link here
