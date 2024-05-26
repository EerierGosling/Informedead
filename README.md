# Informedead

#### Created at the [Apocalypse Hackathon](https://apocalypse.hackclub.com/).

Infromedead is a set of devices that can communicate with each other off the grid!

The prompt for Apocalypse was to create something to help you survive the zombie apocalypse. We thought that since the internet would be unreliable during the apocalypse, it would be helpful to have devices that allow you to communicate regardless!

The code runs on ESP32s, and the devices work by each setting up their own WiFi access points. From there, if a device wants to send a message, it simply connects to the access point of the one it's sending to and sends its message via the WiFi.

All the inputs come from the keypads connected to the ESP32s which are set up somewhat like phone keypads, allowing the user to type any letter or number.

## Device 1
![Device 1, which is in a 3D printed case.](https://github.com/EerierGosling/Informedead/blob/main/images/device_1.JPG)

Device 1 has an LCD and is in a 3D printed case.

## Device 2
![Device 2, which is on a breadboard.](https://github.com/EerierGosling/Informedead/blob/main/images/device_2.JPG)

Due to a lack of materials, device 2 uses an OLED instead of a larger LCD. We weren't able to 3D print its case because there were too many other things 3D printing, so it's on a breadboard instead.

## Device 3
![Device 3, which is just an ESP32.](https://github.com/EerierGosling/Informedead/blob/main/images/device_3.JPG)

Due to there not being enough materials left, device 3 can only receive messages and prints them out to serial.
