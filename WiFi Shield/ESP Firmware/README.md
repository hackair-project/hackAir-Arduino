# ESP Firmware for the Arduino WiFi Shield
This directory houses the ESP firmware's source code along with the configuration landing page.

## Description & Features
The ESP microcontroller is responsible for the following things when using the hackAIR Arduino WiFi shield:

* Manage network connections
* Store AP keys
* Send data received from the Arduino to the hackAIR servers
* Handle WiFi configuration

To handle the WiFi pairing we are using nodemcu's internal 'End-User setup'. The end result can be achieved with
very little code and the page is customizable.

When it's powered on, the ESP checks if there are any known WiFi Access Points in range to connect. If nothing is
available, the ESP will start broadcasting it's own SSID and accept connections from other devices. The user can use
their phone to select a WiFi network for the Arduino to use and input it's password. When instructed by the Arduino, the
ESP will use this WiFi network to transmit gathered data.
 
List of features:
* Full featured HTTP server in AP mode.
* Easy-to-use landing page for configuring WiFi connections
* Handle all network stack duties, leaving the Arduino free to interface with external devices.
* Power saving mode for extending battery life (WIP)

## Install/Use
For users looking to install the firmware to their devices there are tutorials and pre-built images available.
For developers looking to modify the firmware, the development stack is based on Node.JS and the node package manager
(npm). From a terminal at this directory type:

    npm install
    
All required packages for developing will be installed. nodemcu-tool is used to manage writing files to the module.
A couple of npm scripts are available to aid in development:
 * `npm run upload`: Upload everything to the ESP module.
 * `npm run format`: Format the ESP's filesystem.
 * `npm run terminal`: Open a terminal to the ESP module.
 * `npm run build-image`: This will create a `app.bin` file that you can flash using `esptool.py`. This is useful when
 you want to flash both nodemcu and the init.lua in one step. 
 
If uploading is unstable you can try lowering the baud rate at `.nodemcutool`. 9600 is usually a safe bet but it's really slow.
 
## License
