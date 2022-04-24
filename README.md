# esp8266-controller
Application programs for ESP8266 based controller such as esp01, nodemcu, etc.

0. Espalexa Library:
--------------------
Source: https://github.com/Aircoookie/Espalexa

1. alexa-esp8266:
----------------
This application uses Espalexa library and implements a program to control a relay switch over WiFi.
Modify "SSID" and "PASSWORD" in the program where the device will connect to.
This project is done using ESP01 module with an off-the-shelf WiFi relay for ESP01/ESP01S.
Details of hardwares are available below.
This project helped me to switch on/off all of my old electrical appliances without having to buy alexa enabled new ones in just INR 150/-.

To control the device, simply say to alexa: Alexa, turn on Lamp or Alexa, turn off Lamp.
Here, "Lamp" is the name assigned to the module/switch. You can change this in the code as you wish. Check the "device_name" variable.

2. esp-boot-controller:
----------------------
This program implements logic to power-on and power-off a desktop CPU motherboard.
Modify "SSID" and "PASSWORD" in the program where the device will connect to.
This project is done using ESP01 module with an off-the-shelf WiFi relay for ESP01/ESP01S.
Details of hardwares are available below.
This project helped me to switch on/off my normal Desktops over LAN or anywhere from the world (over personal VPN).
This is the cheapest method I can say as it enabled me to remotely control my desktops just within INR 150/-.

After the device is connected to WiFi, a webpage to control on/off can be accessed here: http://ip-address:8080
Or can be controlled through command-line as below:
To power on  CPU: curl -s -X GET http://ip-address:8080/cpu/on
To power off CPU: curl -s -X GET http://ip-address:8080/cpu/off
To reset CPU state: curl -s -X GET http://ip-address:8080/cpu/reset

Here, "ip-address" is the IP assigned to ESP wifi module from your WiFi access-point/router.

The Hardwares:
--------------
ESP01 module:
![](https://www.electronicscomp.com/image/cache/catalog/ai-thinker-esp-01s-esp8266-wifi-module-400x400.jpg)

Relay module:
![](https://m.media-amazon.com/images/I/61FzfFcS68S._SX466_.jpg)

Where to Buy?
-------------
These are very popular modules and can be bought from anywhere, there are several vendors.
Just google for "esp01" or "esp01s" and "esp01 based WiFi relay", and select for the cheapest one.

The best price which I got while writing this README is from Amazon India in just INR 149/-.
This is combined price of ESP01S+Relay in a single purchase.
Link to buy: https://www.amazon.in/Electronic-Spices-ESP8266-ESP-01S-Devlopment/dp/B098X2V9LD/

Circuit:
--------
1. Just plug the esp01/01S module to the relay circuit as shown in the image above.
2. Connect 5V DC supply to power pins of the relay. I had an old USB phone charger and used it.
3. Use the COM and NO pins of the relay to connect your target device. This two pins will short when program logically switch on the target device. You can use the NC pin and COM pin too, but you need to modify the programs accordingly. Refer comments in the program for logic description.

You can find a lot of great tutorials/videos on how to program an ESP01 with arduino, but in case you need any assistant with hardware connection or how to program it, please drop an email to aman2467@gmail.com. I'll be happy to help.
