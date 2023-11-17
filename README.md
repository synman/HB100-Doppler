
# HB100-Doppler
 HB100 "Dopper" Microwave Movement Sensor & ESP32 or ESP8266 HA/MQTT Sensor Publisher
 <center><img width=40% src=https://github.com/synman/HB100-Doppler/assets/1299716/fbd91d69-edd8-4c9f-98d9-c932ccd4ba99></center>

## Attribution
- Various Aliexpress stores - https://www.aliexpress.com/i/2255801172159386.html
- knolleary - https://github.com/knolleary/pubsubclient
- dawidchyrzynskin - https://github.com/dawidchyrzynski/arduino-home-assistant
- me-no-dev - https://github.com/me-no-dev/ESPAsyncWebServer
- ayushsharma82 - https://github.com/ayushsharma82/ElegantOTA
- khoih-prog -  https://github.com/khoih-prog/ESP8266TimerInterrupt
  
## ESP32 & ESP8266 Integration Notes
This is a fairly straight forward setup.  Use VCC (+5V), GND (-), and any free GPIO pin accessible as an interupt (I chose D2).

## Highlights
#### Captive Portal Setup
<img width="60%" alt="Captive Portal" src="https://github.com/synman/BME280/assets/1299716/6f1ab3e9-e20c-4dba-85e3-487158644624">

#### Flexible Configuration Options
<img width="369" alt="Screenshot 2023-10-27 at 8 52 20 PM" src="https://github.com/synman/BME280/assets/1299716/153c9a17-e636-427d-876e-45293bfe459f">

#### Home Assistant Integration via MQTT
<center>
 <img width="60%" alt="Screenshot 2023-10-16 at 2 03 42 PM" src="https://github.com/synman/BME280/assets/1299716/3c02cbec-e4ae-47b1-9cc5-74495fe54dfa">
 <img width="60%" alt="Screenshot 2023-10-16 at 2 05 01 PM" src="https://github.com/synman/BME280/assets/1299716/99f3e628-f59b-4bcb-8821-e54adf47c0ab">
</center>

#### Filesystem & Flash Web OTA
<img width="364" alt="Screenshot 2023-10-27 at 8 52 48 PM" src="https://github.com/synman/BME280/assets/1299716/c3ef6776-ac74-46e9-88e6-7a2656217d5e">
