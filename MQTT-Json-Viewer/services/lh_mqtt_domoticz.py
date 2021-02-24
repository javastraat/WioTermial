#!/usr/bin/env python3
import paho.mqtt.client as mqtt
import json
import os
import time
import sys
import pathlib
import urllib.request

# Settings
dom_idx=455
#sub_mqtt="Home/snespot/#"
sub_mqtt="Home/MMDVMBridge/LastHeard"
pub_mqtt_1="domoticz/in"
pub_mqtt="Home/MMDVMBridge/data"
mqtt_server_ip="192.168.x.xxx"
#database='/opt/MMDVM_Bridge/DMRIds.dat'
#url='https://raw.githubusercontent.com/DMR-Database/database-beta/master/DMRIds.dat'
url='https://raw.githubusercontent.com/DMR-Database/database-beta/master/usr.bin'
dirdb=os.getcwd()
#database=dirdb+"/DMRIds.dat"
database=dirdb+"/usr.bin"
#print(dirdb)
#
#check for DMRIds.dat else download from github
#
file = pathlib.Path("usr.bin")
if file.exists ():
    print ("Database exists continuing")
else:
    print ("File not exist")
    print("Downloading DMRIds.dat from", url)
    urllib.request.urlretrieve(url,database)
    print()
    print("Please restart python script")
    exit()
#
#
#This is the Subscriber connect
def on_connect(client, userdata, flags, rc):
    print ("--------------------------")
    if rc==0:
     print("Connection sucessfull")
    sys.stdout.flush()
#    print("Connected with result code "+str(rc))
    print("MQTT Server IP :",mqtt_server_ip)
    sys.stdout.flush()
    print("Feed Subscribe :",sub_mqtt)
    sys.stdout.flush()
    print("Feed Publish   :",pub_mqtt)
    sys.stdout.flush()
    print("Domoticz IDX   :",dom_idx)
    sys.stdout.flush()
    print("DMRIds.dat     :",database)
    sys.stdout.flush()
    client.subscribe(sub_mqtt)
    print("--------------------------")
    sys.stdout.flush()
    print("waiting for new message...")
    sys.stdout.flush()


def on_message(client, userdata, message):
#    print("message received :",str(message.payload.decode("utf-8")),\
#          "topic",message.topic,"retained ",message.retain)
#    if message.retain==1:
#        print("This is a retained message")
#

    m=str(message.payload.decode("utf-8"))
#    n=({'command': 'udevice', 'idx' : (dom_idx), 'svalue' : (m)})
    d = json.dumps({'dmrid': '2041152', 'callsign': (m), 'name': 'Test', 'town': 'Amsterdam','county': 'NH', 'country': 'NL '})


#lastheard :  WB4FWQ
#send to mqtt :  {'command': 'udevice', 'idx': 455, 'svalue': 'WB4FWQ'}
#    print (type(m))
#    print (type(n))
#    print (type(d))
#    print ("Message received :",(m))
#    print ("Json data : ",(d))
#    print ("Send to",pub_mqtt,":",(d))
#    print ("Publishing...")
    searchfile = open(database, "r")
    for line in searchfile:
        if m in line:
#          dump=json.dumps({'command': 'udevice', 'idx' : (dom_idx), 'svalue' :(line)})
          dmrid, callsign, name, town, county, country = line.split(',')
          d = json.dumps({'dmrid': (dmrid), 'callsign': (m), 'name': (name), 'town': (town),'county': (county), 'country': (country)})
          n = (callsign)+" "+(name)
          dump=json.dumps({'command': 'udevice', 'idx' : (dom_idx), 'svalue' :(n)})

#          print(n)
#          print(dmrid) 
#          print(callsign) 
#          print(name) 
#          print(town) 
#          print(county) 
#          print(country)



##           print(line)
#          print ("Publishing...")
#          sys.stdout.flush()
    client.publish(pub_mqtt,d)
#    client.publish(pub_mqtt_1,dump)
    searchfile.close()
#    print ("--------------------------")
#    print ("waiting for new message...")


client = mqtt.Client()
client.connect(mqtt_server_ip,1883,60)

client.on_connect = on_connect
client.on_message = on_message


client.loop_forever()
