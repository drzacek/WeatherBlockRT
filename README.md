# WeatherBlockRT
A mini indoor weather station with display


A project of a weather-clock - intended for use with stm32 blue pill and tft 2.2 SPI display.

1. Hardware:

- stm32f103xx "blue pill" 32 bit microcontroller
- DHT22 Temperature/Humidity sensor
- DS3231 real-time clock
- ILI9341 SPI 2.2 TFT Display (240x320)
- ESP8266 module


2. Software / functions:

- read current temperature/humidity from DHT22 sensor
- read/set current date/time from real time clock
- display time, date, temp and hum on the display
- send the information (hum/temp) via esp8266 to mqtt broker (to use with homeassistant and similar)
- use virtual COM port to debug
  - read all sensor values
  - set time/date
  - set wifi connection
  
  
 # Current state

 - hardware project more or less done (cubeMX project)
 - reading humidity and temperature from DHT22 works 
 - reading and setting time works
 - Virtual COM port works (can send commands, commands come back, can set time, read sensor data)
 
 
@TODO
 - reading temp/humidity sometimes delivers false values (happens too often?)
 - whole DISPLAY functionality
 - wifi connection and communication with esp8266
