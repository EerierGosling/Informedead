# Informedead

#### Created at the [Apocalypse Hackathon](https://apocalypse.hackclub.com/).

Infromedead is a set of devices that can communicate with each other off the grid!

The prompt for Apocalypse was to create something to help you survive the zombie apocalypse. We thought that since the internet would be unreliable during the apocalypse, it would be helpful to have devices that allow you to communicate regardless!

The code runs on ESP32s, and the devices work by each setting up their own WiFi access points. From there, if a device wants to send a message, it simply connects to the access point of the one it's sending to and sends its message via the WiFi.

All the inputs come from the keypads connected to the ESP32s which are used like phone keypads, allowing the user to type any letter or number.
