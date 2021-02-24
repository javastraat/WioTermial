# WioTermial

MQTT and Json viewer

Basic Usage : Use Wio Terminal to see MQTT Data 

Extended usage :
Use Wio Terminal to see lastheard data (DMR) on radioamateur Brandmeister network.
Use Wio Terminal to see Domoticz data from domoticz/out.

<hr>

Components:
-MQTT example
https://github.com/salmanfarisvp/Wio-Terminal-MQTT

-Json processing (JsonParserExample)
https://arduinojson.org/v6/example/parser/

-NTP Timeclient (for timestamps and clock)
https://github.com/arduino-libraries/NTPClient/tree/master/examples/Advanced

-ExtFlashLoader (for sd menu)
https://github.com/ciniml/ExtFlashLoader

-alot of code found on github and google

--------------------------

Options during boot:
-Button A (right):  Escape to ExtFlashLoader Menu

Options after full start:
-Button A (right):  Escape to ExtFlash Menu
-Button B (middle): nothing yet (planned to move domoticz here)
-Button C (left):   Helppage

-Joystick left : Basic MQTT (plain) output (default)
-Joystick up   : Nice MQTT output (mqtt contains json output)
-Joystick right: Extended MQTT output (mqtt contains json output) 
-Joystick down : Domoticz MQTT output (mqtt contains json output)  
