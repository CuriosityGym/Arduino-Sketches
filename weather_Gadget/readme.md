 
# **Weather Gadget**
   
   In this example we are using ESP8266 wifi module and OLED display
 to display weather conditinos of particular city from WeatherDataApi.
              
 WeatherDataApi: Heroku Deployment Application that reduces data 
                 downloaded for OpenWeatherMap Api
  
 To get data from OpenWeatherMap Api, you need to create openweathermap 
 account and then generate API key and use that API Key in this code.                
 In this example we have used ELClient.h library to get data from WeatherDataApi. 
 Here we are sending request for location,current temprature,humnidity and 
 weather descrition. This code allows you to detect your location automatically
 by IP address or to get weather conditions of particular city just change  the 
 city ID in the following code and change autoDetectLocation to false. OLED is 
 used to display this information.
 We are sending request to "idiotware.herokuapp.com" after every one hour to get
 weather information.To change this interval, change value of refreshRate.
