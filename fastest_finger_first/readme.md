#**idIoTware Shield Fastest Finger First**
#####**Attention: Please install all libraries from our Github Repository to enable this example to run.**
  
  In this example we are using NRF24L01 radio module, Pushbutton switch, Buzzer and an I2C OLED display to show which player pressed pushbutton first.
  Here, 4 shields act as Transmitter and One shield act as a Receiver. Each time when any user(transmitter) presses button, then arduino
  sends the user number to receiver through NRF24L01 radio module and receiver receives the input and makes decision which player pressed
  the button first.You can change the number of players and particular time after which receiver will take decision.
  
  In this example we have used RF24Network.h and RF24.h library for NRF24L01 radio module and u8glib.h library for OLED display.
  This sketch is for receiver. In this example receiver receives input from users and decides which user pressed the pushbutton first and shows the 
  number of that user on OLED display and makes beep sound on buzzer.
  
#####**Setup:**

1.create account on thingspeak and channel
2.create account on IFTTT, connect to maker channel and make recipe.
3.replace API key for thingspek channel and maker channel
4.install all libraries from our Github Repository to enable this example to run.
5.connect sensors to proper input pins and caliberate the sensors for perfect reading.
#####**Precautions:**

1.Use mosfet or relay module to drive motor.
2.keep sensor properly in soil to get proper readings
#####**Images:** put link here
#####**Video:** put link here
