# Wio Terminal MQTT Json viewer	

<center>	
Install Mosquitto MQTT Broker/Server if not done before.<br>	
http://www.steves-internet-guide.com/install-mosquitto-linux/<br>	
<br><br>	
Booting Wio Terminal<br>	
<img src="images/start.jpeg" width="300">	
<br><br>	
Simple display usage : <br>	
Use Wio Terminal to see MQTT Data <br>	
- our data is made by monitoring mmdvmhost logfile.<br>	
<img src="images/simple.jpeg" width="300">	
<br><br>	
Basic display usage :<br>	
Use Wio Terminal to see lastheard data (mqtt-json) from radioamateur Brandmeister network.<br>	
-<b>This needs monitor and mqttbridge service and a running MMDVMHost instance (Hotspot, Bridge etc)</b><br>	
-Getting notified when friend is talking on network (text turns red and wio terminal beeps two times).<br>	
<img src="images/basic.jpeg" width="300">	
<br><br>	
Extended display usage (display all data):<br>	
<img src="images/extended.jpeg" width="300"><br>	
<br><br>	
Domoticz data display (mqtt-json).<br>	
-<b>This needs domoticz with mqtt plugin to generate data on domoticz/out</b><br>	
<img src="images/domoticz.jpeg" width="300"><br>	
<hr>	
Options during boot:<br>	
-Button A (right):  Escape to ExtFlashLoader Menu	
<br><br>	
Options after full start:<br>	
-Button A (right):  Escape to ExtFlash Menu<br>	
-Button B (middle): nothing yet (planned to move domoticz here)<br>	
-Button C (left):   Helppage (press again to exit)	
<br><br>	
-Joystick left : Basic MQTT (plain) output (default)<br>	
-Joystick up   : Nice MQTT output (mqtt contains json output)<br>	
-Joystick right: Extended MQTT output (mqtt contains json output) <br>	
-Joystick down : Domoticz MQTT output (mqtt contains json output)	
<br><br>	
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
