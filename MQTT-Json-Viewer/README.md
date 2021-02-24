# WioTermial

Wio Terminal MQTT and Json viewer<br>
<br>
Basic Usage : <br>
Use Wio Terminal to see MQTT Data <br>
<br>
Extended usage :<br>
Use Wio Terminal to see lastheard data (DMR) on radioamateur Brandmeister network.<br>
Getting notified when friend is talking on network.<br>
<br>
Use Wio Terminal to see Domoticz data from domoticz/out.<br>
<hr>
Options during boot:<br>
-Button A (right):  Escape to ExtFlashLoader Menu<br>
<br>
Options after full start:<br>
-Button A (right):  Escape to ExtFlash Menu<br>
-Button B (middle): nothing yet (planned to move domoticz here)<br>
-Button C (left):   Helppage (press again to exit)<br>
<br>
-Joystick left : Basic MQTT (plain) output (default)<br>
-Joystick up   : Nice MQTT output (mqtt contains json output)<br>
-Joystick right: Extended MQTT output (mqtt contains json output) <br>
-Joystick down : Domoticz MQTT output (mqtt contains json output)  <br>
<br>


<hr>
Thanx to :<br><br>
Components added to code:<br>
-MQTT example<br>
https://github.com/salmanfarisvp/Wio-Terminal-MQTT<br>
<br>
-Json processing (JsonParserExample)<br>
https://arduinojson.org/v6/example/parser/<br>
<br>
-NTP Timeclient (for timestamps and clock)<br>
https://github.com/arduino-libraries/NTPClient/tree/master/examples/Advanced<br>
<br>
-ExtFlashLoader (for sd menu)<br>
https://github.com/ciniml/ExtFlashLoader<br>
<br>
-alot of code found in other samples,on github and google.<br>
<br>
if your name should go here -> tell me. (sorry i forgot you)
