# WioTermial

MQTT and Json viewer

Basic Usage : Use Wio Terminal to see MQTT Data 

Extended usage :
Use Wio Terminal to see lastheard data (DMR) on radioamateur Brandmeister network.<br>
Use Wio Terminal to see Domoticz data from domoticz/out.

<hr>
Components added to code:
-MQTT example
https://github.com/salmanfarisvp/Wio-Terminal-MQTT

-Json processing (JsonParserExample)
https://arduinojson.org/v6/example/parser/

-NTP Timeclient (for timestamps and clock)
https://github.com/arduino-libraries/NTPClient/tree/master/examples/Advanced

-ExtFlashLoader (for sd menu)
https://github.com/ciniml/ExtFlashLoader

-alot of code found in other samples,on github and google.<br>

if your name should go here -> tell me. (sorry i forgot you)
<hr>
Options during boot:<br>
-Button A (right):  Escape to ExtFlashLoader Menu<br>
<br><br>
Options after full start:<br>
-Button A (right):  Escape to ExtFlash Menu<br>
-Button B (middle): nothing yet (planned to move domoticz here)<br>
-Button C (left):   Helppage<br>
<br><br>
-Joystick left : Basic MQTT (plain) output (default)<br>
-Joystick up   : Nice MQTT output (mqtt contains json output)<br>
-Joystick right: Extended MQTT output (mqtt contains json output) <br>
-Joystick down : Domoticz MQTT output (mqtt contains json output)  <br>
<br><br>

