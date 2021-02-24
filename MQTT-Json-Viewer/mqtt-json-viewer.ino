
/*
   MQTT Exmple for SeeedStudio Wio Terminal
   Author: Salman Faris
   Date: 31/07/2020
   Last Updates: 02/08/2020

   MQTT Broker broker.mqtt-dashboard.com
   Subscribe Topic Name: WTIn
   Publish Topic Name: WTout
  - publishes "hello world" to the topic "WTout"
  - subscribes to the topic "WTin", printing out any messages

  NTPClient : https://github.com/arduino-libraries/NTPClient
  ExtFlash https://github.com/ciniml/ExtFlashLoader

*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <rpcWiFi.h>
#include"TFT_eSPI.h"
#include <PubSubClient.h>
#include <SPI.h>
//include for menu
#include <cstdint>
#include <ExtFlashLoader.h>
//include json
//#include "Free_Fonts.h"
#include <ArduinoJson.h>

//
//include ntp client
//
#include <NTPClient.h> //https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
//
//
const char* ver_txt = "1.21.02.24";
const char* author = "PD2EMC";
int help_delay = 5000;
//
//
// Update these with values suitable for your network.
const char* ssid = "einstein.amsterdam"; // WiFi Name
const char* password = "password";  // WiFi Password
const char* mqtt_server = "192.168.x.xxx";  // MQTT Broker URL

int mqtt_conn = 0; //Placeholder 0=sub_mqtt_0,1=sub_mqtt_1 etc etc (topic selection)
int mqtt_json = 0; //set in server check in setup() - placeholder
int mqtt_conn_override = 99; //override

//
//subscribtions
const char* sub_mqtt = ""; //placeholder
const char* sub_mqtt_0 = "Home/MMDVMBridge/LastHeard"; //subscribe to topic (default topic) left button
const char* sub_mqtt_1 = "Home/MMDVMBridge/data"; //subscribe to topic - no json
const char* sub_mqtt_2 = "domoticz/out"; //subscribe to topic - json

const char* menu_txt   = "Exit to SD menu";
const char* menu_txt_0 = "Simple/Basic/Extended View";
const char* menu_txt_1 = "Domoticz MQTT JSON Viewer";
const char* menu_txt_2 = "Help Menu";
const char* menu_txt_3   = "Exit to SD menu";

const char* menu_topic = "Starting MQTT Json Viewer"; //placeholder
const char* menu_topic_0 = "MMDVM Simple MQTT Viewer";
const char* menu_topic_1 = "MMDVM Basic MQTT Viewer";
const char* menu_topic_2 = "MMDVM Extend MQTT Viewer";
const char* menu_topic_3 = "Domoticz MQTT Json Viewer";


//
//Size and location
int sub_json_size = 2; //size of json output
int sub_json_loc = 20; //position of json output
int sub_mqtt_size = 6; //size of normal output
int sub_mqtt_loc = 100; //position of normal output
//

//publish alive message
const char* pub_mqtt = "Home/MMDVMBridge/listener"; //publish in topic
const char* pub_msg = "WioTerminal-"; // publish text + random hex after boot f.e. "WioTerminal-4c7f"




TFT_eSPI tft;

WiFiClient wioClient;
PubSubClient client(wioClient);
long lastMsg = 0;
char msg[50];
int value = 0;


void setup_wifi() {

  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.print("MQTT Viewer by ");
  Serial.print(author);
  Serial.print(" ver: ");
  Serial.println(ver_txt);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //
  //
  tft.setTextSize(2);
  header();
  footer();
  tft.setCursor(5, 30);
  tft.print("Connecting to Wi-Fi ");
  WiFi.begin(ssid, password); // Connecting WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tft.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Display Local IP Address
  Serial.println("");
  //
  tft.setCursor(5, 50);
  tft.setTextColor(TFT_GREEN);
  tft.println("-Wifi connected!");
  tft.setTextColor(TFT_WHITE);
  //delay(1000);
  //tft.fillScreen(TFT_BLACK);
  //header();
  //tft.setCursor(5, 120);
  //tft.setTextSize(2);
  //tft.print("waiting for next message");
  //footer();
}

void callback(char* topic, byte* payload, unsigned int length) {
  //
  //blank middle
  //tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char buff_p[length];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    buff_p[i] = (char)payload[i];
  }
  Serial.println();
  buff_p[length] = '\0';
  String msg_p = String(buff_p);
  //tft.fillScreen(TFT_BLACK); // blank screen before new data is displayed
  //
  //msg counter//
  ++value;
  snprintf (msg, 50, "%ld", value);
  //
  //
  msg_p.trim();
  //
  // main screen header
  header();
  //
  // check for friends (text) and beep x times alarm(value);
  if (msg_p == "PD2EMC" || msg_p == "PD1WP" || msg_p == "N2XRV" || msg_p == "PD1LOI" || msg_p == "PH4SE" || msg_p == "PI4ASD" || msg_p == "Test") {
    //  tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_RED);
    alarm(3);
  }
  tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
  //middle section
  //tft.setTextSize(sub_mqtt_size);
  //tft.setCursor((320 - tft.textWidth(msg_p)) / 2, sub_mqtt_loc);
  //tft.print(msg_p); // Print receved payload
  //
  // DMRID json start
  //basic view
  if ((mqtt_json) == 1) { //for dmrid data
    tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    tft.setTextSize(sub_json_size);
    //
    StaticJsonDocument<344> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, msg_p);
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    // Fetch values.
    //
    // Most of the time, you can rely on the implicit casts.
    // In other case, you can do doc["time"].as<long>();
    const char* json_dmrid = doc["dmrid"];
    const char* json_callsign = doc["callsign"];
    const char* json_name = doc["name"];
    const char* json_town = doc["town"];
    const char* json_county = doc["county"];
    const char* json_country = doc["country"];
    //
    if (!strcmp(json_callsign, "PD2EMC")) {
      //  tft.fillScreen(TFT_RED);
      tft.setTextColor(TFT_RED);
      Serial.print("callsign:  ");
      Serial.println(json_callsign);
      alarm(3);
    }
    //delay(100);
    //tft.setTextSize(sub_mqtt_size);
    tft.setTextSize(8);
    tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    tft.setCursor((320 - tft.textWidth(json_callsign)) / 2, 45); //place text in middle
    tft.print(json_callsign); // Print received payload

    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(4);
    tft.setCursor((320 - tft.textWidth(json_name)) / 2, 110); //place text in middle
    tft.print(json_name); // Print received payload

    tft.setTextSize(2);
    tft.setCursor(5, 150);
    tft.print("Town: ");
    tft.println(json_town);
    tft.setCursor(5, 170);
    tft.print("State: ");
    tft.println(json_county);
    tft.setCursor(5, 190);
    tft.print("Country: ");
    tft.println(json_country);
  }

  //
  // DMRID json end
  //
  // DMRID json start
  // extended view
  if ((mqtt_json) == 2) { //for dmrid data
    tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    //tft.setCursor(0, (sub_mqtt_loc));
    tft.setTextSize(sub_json_size);
    //msg_p.replace("Domoticz", "Domo");
    //msg_p.replace("Buienradar", "BR");
    //msg_p.replace("_", " ");
    StaticJsonDocument<344> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, msg_p);

    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    // Fetch values.
    //
    // Most of the time, you can rely on the implicit casts.
    // In other case, you can do doc["time"].as<long>();
    const char* json_dmrid = doc["dmrid"];
    const char* json_callsign = doc["callsign"];
    const char* json_name = doc["name"];
    const char* json_town = doc["town"];
    const char* json_county = doc["county"];
    const char* json_country = doc["country"];

    // double latitude = doc["data"][0];
    // double longitude = doc["data"][1];
    //tft.fillScreen(TFT_BLACK);
    //delay(100);

    tft.setCursor(15, 40);
    tft.print("DMRid: ");
    tft.println(json_dmrid);
    tft.setCursor(15, 70);
    tft.print("Callsign: ");
    tft.println(json_callsign);
    tft.setCursor(15, 100);
    tft.print("Name: ");
    tft.println(json_name);
    tft.setCursor(15, 130);
    tft.print("Town: ");
    tft.println(json_town);
    tft.setCursor(15, 160);
    tft.print("State: ");
    tft.println(json_county);
    tft.setCursor(15, 190);
    tft.print("Country: ");
    tft.println(json_country);

    // Print values.
    //Serial.print("idx:   ");
    //Serial.println(json_idx);
    //Serial.print("name:  ");
    //Serial.println(json_name);
    //Serial.print("type:  ");
    //Serial.println(json_stype);
    //Serial.print("value: ");
    //Serial.println(json_val);
    //Serial.println(time);
    //Serial.println(latitude, 6);
    //Serial.println(longitude, 6);
  }
  //
  // DMRID json end
  //
  // Domoticz json start
  //
  else if ((mqtt_json) == 3) {
    tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    //tft.setCursor(0, (sub_mqtt_loc));
    tft.setTextSize(sub_json_size);
    msg_p.replace("Domoticz", "Domo");
    msg_p.replace("Buienradar", "BR");
    msg_p.replace("_", " ");
    StaticJsonDocument<344> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, msg_p);

    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    // Fetch values.
    //
    // Most of the time, you can rely on the implicit casts.
    // In other case, you can do doc["time"].as<long>();
    long json_idx = doc["idx"];
    const char* json_name = doc["name"];
    const char* json_stype = doc["stype"];
    double json_val = doc["svalue1"];

    // double latitude = doc["data"][0];
    // double longitude = doc["data"][1];
    //tft.fillScreen(TFT_BLACK);
    delay(100);

    tft.setTextColor(TFT_WHITE);

    tft.setTextSize(3);
    tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    tft.setCursor((320 - tft.textWidth(json_name)) / 2, 45); //place text in middle
    tft.print(json_name); // Print received payload

    tft.setTextSize(3);
    tft.setCursor((320 - tft.textWidth(json_stype)) / 2, 90); //place text in middle
    tft.print(json_stype); // Print received payload

    tft.setTextSize(4);
    tft.setCursor(120, 145);
    //tft.setCursor((320 - tft.textWidth(json_val)) / 2, 120); //place text in middle
    tft.print(json_val); // Print received payload

    //tft.setCursor(5, 50);
    //tft.print("IDX: ");
    //tft.println(json_idx);
    //tft.setCursor(5, 80);
    //tft.print("Name: ");
    //tft.println(json_name);
    //tft.setCursor(5, 160);
    //tft.print("Type: ");
    //tft.println(json_stype);
    //tft.setCursor(5, 180);
    //tft.print("Value: ");
    //tft.println(json_val);

    // Print values.
    Serial.print("idx:   ");
    Serial.println(json_idx);
    Serial.print("name:  ");
    Serial.println(json_name);
    Serial.print("type:  ");
    Serial.println(json_stype);
    Serial.print("value: ");
    Serial.println(json_val);
    //Serial.println(time);
    //Serial.println(latitude, 6);
    //Serial.println(longitude, 6);



  }
  // middle json end
  //
  //
  //start center text no json plain text
  // simple view
  if ((mqtt_json) == 0) {
    //tft.setCursor(30, 100);
    tft.setTextSize(sub_mqtt_size);
    tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    tft.setCursor((320 - tft.textWidth(msg_p)) / 2, sub_mqtt_loc); //place text in middle
    tft.print(msg_p); // Print received payload
    tft.setTextColor(TFT_WHITE);
  }


  //  tft.setCursor((320 - tft.textWidth(msg_p)) / 2, (sub_mqtt_loc)); //place text in middle
  // tft.setFreeFont(FMB12);
  // tft.setCursor(0, (sub_mqtt_loc));
  //  msg_p.trim();
  //msg_p.replace("{", "");
  //msg_p.replace("}", "");
  //msg_p.replace(",", "");
  //msg_p.replace("", "");
  //msg_p.replace(" . ", " ");
  //msg_p.replace(" : ", ":");
  //msg_p.replace(" - ", "-");
  //  msg_p.replace('"', "");


  //  tft.print(msg_p); // Print received payload

  footer();

}


void header() {
  // main display header
  //
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.fillRect(0, 0, 320, 28, TFT_BLACK);//x,y w,h color
  tft.setCursor((320 - tft.textWidth(menu_topic)) / 2, 5);
  //tft.setCursor(0, 5);
  tft.print(menu_topic);
  //redline
  tft.drawFastHLine(0, 25, 320 , TFT_RED);
  tft.drawFastHLine(0, 26, 320 , TFT_RED);
  tft.drawFastHLine(0, 27, 320 , TFT_RED);
  //clear middle
  //tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
}
//
// footer
void footer() {
  // main display footer
  tft.setTextSize(2);
  tft.fillRect(0, 212, 320, 28, TFT_BLACK);//x,y w,h color
  ///  tft.setCursor((320 - tft.textWidth(25) / 2, 215); //place text in middle
  //redline
  tft.drawFastHLine(0, 213, 320 , TFT_RED);
  tft.drawFastHLine(0, 214, 320 , TFT_RED);
  tft.drawFastHLine(0, 215, 320 , TFT_RED);
  tft.setCursor(5, 220);
  //tft.setCursor((320 - tft.textWidth(23)) / 2, 200);
  // tft.setFreeFont(FMB12);
  //tft.print("Received messages: ");
  tft.setCursor(5, 220);
  tft.fillRect(0, 220, 320, 20, TFT_BLACK);

  tft.print("Messages rec: ");
  tft.print(msg); // Print received message count
  tft.setCursor(220, 220);
  tft.print(timeClient.getFormattedTime());

  //
  Serial.print("MQTT messages received: ");
  Serial.println(msg);
  Serial.println(timeClient.getFormattedTime());
  tft.setTextColor(TFT_WHITE);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    //    tft.setCursor((320 - tft.textWidth("Attempting MQTT connection")) / 2, 60);
    tft.setCursor(5, 70);
    tft.print("Connecting to MQTT...");
    delay(1000);
    // Create a random client ID
    String clientId = pub_msg;
    clientId += String(random(0xffff), HEX);
    clientId += "-";
    clientId += (timeClient.getFormattedTime());

    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("MQTT connected");
      //      tft.setCursor((320 - tft.textWidth("MQTT connected")) / 2, 80);
      tft.setCursor(5, 90);
      tft.setTextColor(TFT_GREEN);
      tft.print("-MQTT connected!");
      tft.setTextColor(TFT_WHITE);
      // Once connected, publish an announcement...
      tft.setCursor(5, 110);
      tft.println("Publish announcement...");
      delay(1000);
      client.publish(pub_mqtt, clientId.c_str());//pub_msg+hex string+time
      //      tft.setCursor((320 - tft.textWidth("publish an announcement")) / 2, 110);
      tft.setCursor(5, 130);
      tft.setTextColor(TFT_GREEN);
      tft.println("-Announcement published!");
      tft.setTextColor(TFT_WHITE);
      delay(1000);
      // ... and resubscribe
      tft.setCursor(5, 150);
      tft.println("Subscribe to topic...");
      client.subscribe(sub_mqtt);
      //tft.setCursor(5, 180);
      //tft.println(sub_mqtt);
      delay(1000);
      tft.setCursor(5, 170);
      tft.setTextColor(TFT_GREEN);
      tft.println("-Subscribed to topic!");
      tft.setTextColor(TFT_WHITE);
      //      tft.setCursor((320 - tft.textWidth("waiting for MQTT message")) / 2, 130);
      tft.setCursor(5, 190);
      tft.println("Waiting for MQTT messages");
      //footer();
      delay(1000);
      //tft.fillScreen(TFT_BLACK);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void alarm(int x) {
  for (int i = 0; i < x; i++) {
    analogWrite(WIO_BUZZER, 150);
    delay(80);
    analogWrite(WIO_BUZZER, 0);
    delay(80);
  }
}

void helpscreen() {
  //
  //Help screen pulled by left button loop()
  //
  delay(100);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  //header
  tft.setCursor((320 - tft.textWidth("- MQTT json Viewer Help -")) / 2, 5);
  tft.print("- MQTT json Viewer Help -");
  //redline
  tft.drawFastHLine(0, 24, 320 , TFT_RED);
  tft.drawFastHLine(0, 25, 320 , TFT_RED);
  tft.drawFastHLine(0, 26, 320 , TFT_RED);
  //menutext
  tft.setCursor(5, 35);
  tft.print("Joystick left/up/right:");
  tft.setCursor(5, 55);
  tft.print(menu_txt_0);
  //
  tft.setCursor(5, 80);
  tft.print("Joystick Down:");
  tft.setCursor(5, 100);
  tft.print(menu_txt_1);
  //
  tft.setCursor(5, 125);
  tft.print("Button left:");
  tft.setCursor(5, 145);
  tft.print(menu_txt_2);
  //
  tft.setCursor(5, 170);
  tft.print("Button right:");
  tft.setCursor(5, 190);
  tft.print(menu_txt_3);
  //redline
  tft.drawFastHLine(0, 213, 320 , TFT_RED);
  tft.drawFastHLine(0, 214, 320 , TFT_RED);
  tft.drawFastHLine(0, 215, 320 , TFT_RED);
  //footer
  tft.setCursor(10, 220); //fixed position
  tft.print("By ");
  tft.print(author);
  tft.print(" ver. ");
  tft.print(ver_txt);
  // wait till c is pressed again
  while ( digitalRead(WIO_KEY_C) == HIGH) {

  }
  //  delay(help_delay);
  tft.fillScreen(TFT_BLACK);
  header();
  tft.setCursor(5, 120);
  tft.setTextSize(2);
  tft.print(" waiting for next message");
  footer();
  //tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
  //clear screen afer wait
  //tft.fillScreen(TFT_BLACK);
  //
  //end welcome screen

}

void setup() {
  timeClient.begin();
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(3);
  //tft.fillScreen(TFT_BLACK);
  //helpscreen();

  //
  //Button sequence
  //https://github.com/ciniml/ExtFlashLoader
  //
  //press right button to escape to menu
  pinMode(WIO_BUZZER, OUTPUT); //Set buzzer pin as OUTPUT
  pinMode(WIO_KEY_A, INPUT_PULLUP); //right
  pinMode(WIO_KEY_B, INPUT_PULLUP); //middle
  pinMode(WIO_KEY_C, INPUT_PULLUP); //left
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  //

  if ( digitalRead(WIO_KEY_A) == LOW) {//right button
    alarm(3);
    tft.setCursor(5, 120);
    tft.setTextSize(2);
    tft.print("Launching menu application");
    delay(1500);
    tft.fillScreen(TFT_BLACK);
    ExtFlashLoader::ExtFlashLoader loader;
  }
  // press middle during boot for domoticz (replaced with up in loop)
  // else if ( digitalRead(WIO_KEY_B) == LOW) {//middle button domoticz
  //   alarm(2);
  //   mqtt_conn = 2; //change to topic 2
  // }
  //press left for json full dmr info
  //else if ( digitalRead(WIO_KEY_C) == LOW) {//left button
  //  alarm(1);
  //  mqtt_conn = 1; //change to topic 1
  //}
  //else {
  //alarm(1);
  //mqtt_conn = 0; //change to topic 0
  //}
  //
  //check server setting
  //mqtt_conn = 0; in header
  //
  // if (mqtt_conn == 0 || mqtt_conn_override == 0) {
  mqtt_json = 0; //if output is json/xml = 1
  sub_mqtt = sub_mqtt_0;//select subscription 0
  //  menu_topic = menu_topic_0;
  //tft.setCursor(5, 120);
  //tft.setTextSize(2);
  //tft.print("Starting ");
  //tft.print(menu_txt_0);
  //delay(1500);
  //tft.fillScreen(TFT_BLACK);


  //}
  // if (mqtt_conn == 1 || mqtt_conn_override == 1) {
  //   mqtt_json = 1; //if output is json/xml = 1
  //   sub_mqtt = sub_mqtt_1;//select subscription 1
  //   menu_topic = menu_topic_1;
  //   tft.setCursor(5, 120);
  //   tft.setTextSize(2);
  //   tft.print("Starting ");
  //   tft.print(menu_txt_1);
  //   delay(1500);
  //   tft.fillScreen(TFT_BLACK);
  // }
  // press middle during boot for domoticz (replaced with up in loop)
  //if (mqtt_conn == 2 || mqtt_conn_override == 2) {
  //  mqtt_json = 2; //if output is json/xml = 1
  //  sub_mqtt = sub_mqtt_2;//select subscription 2
  //  menu_topic = menu_topic_2;
  //  tft.setCursor(5, 120);
  //  tft.setTextSize(2);
  //  tft.print("Starting ");
  //  tft.print(menu_txt_2);
  //  delay(1500);
  //  tft.fillScreen(TFT_BLACK);
  //}
  //end server check

  Serial.println();
  Serial.begin(115200);
  setup_wifi();
  menu_topic = menu_topic_0;
  client.setServer(mqtt_server, 1883); // Connect the MQTT Server
  client.setCallback(callback);
}

void loop() {
  timeClient.update();
  //
  // tft.fillRect(219, 220, 320, 20, TFT_BLACK);
  // tft.setCursor(220, 220);
  // tft.print(timeClient.getFormattedTime());
  //
  // Button press detector
  //
  // press top right button to escape to extflash menu
  //
  if ( digitalRead(WIO_KEY_A) == LOW) {//right button
    alarm(1);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(5, 120);
    tft.setTextSize(2);
    tft.print("Starting SD Card menu");
    delay(1500);
    // tft.fillScreen(TFT_BLACK);
    ExtFlashLoader::ExtFlashLoader loader;
  }
  // press top left button for help
  //
  if ( digitalRead(WIO_KEY_C) == LOW) {//left button
    alarm(1);
    tft.fillScreen(TFT_BLACK);
    helpscreen();
  }
  //
  // press left joypad for simple dmr info (callsign only)
  if ( digitalRead(WIO_5S_LEFT) == LOW) {
    alarm(1);
    tft.fillScreen(TFT_BLACK);
    client.unsubscribe(sub_mqtt);
    mqtt_conn = 0; //change to topic 2
    mqtt_json = 0; //if output is json/xml = 1
    sub_mqtt = sub_mqtt_0;//select subscription 0
    menu_topic = menu_topic_0;
    header();
    tft.setCursor(5, 120);
    tft.setTextSize(2);
    tft.print("waiting for next message");
    footer();
    delay(1500);
    //tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    // tft.fillScreen(TFT_BLACK);
    //setup_wifi();
    //client.setServer(mqtt_server, 1883);
    client.subscribe(sub_mqtt);
    //
  }
  //
  // press up joypad for basic dmr info
  if ( digitalRead(WIO_5S_UP) == LOW) {//right button
    alarm(1);
    tft.fillScreen(TFT_BLACK);
    client.unsubscribe(sub_mqtt);
    mqtt_conn = 1; //change to topic 1
    mqtt_json = 1; //if output is json/xml = 1
    sub_mqtt = sub_mqtt_1;//select subscription 1
    menu_topic = menu_topic_1;
    header();
    tft.setCursor(5, 120);
    tft.setTextSize(2);
    tft.print("waiting for next message");
    footer();
    delay(1500);
    //tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    //tft.fillScreen(TFT_BLACK);
    //setup_wifi();
    //client.setServer(mqtt_server, 1883);
    client.subscribe(sub_mqtt);
    //
  }//
  // press right joypad for extended dmr info
  if ( digitalRead(WIO_5S_RIGHT) == LOW) {//right button
    alarm(1);
    tft.fillScreen(TFT_BLACK);
    client.unsubscribe(sub_mqtt);
    mqtt_conn = 1; //change to topic 1
    mqtt_json = 2; //if output is json/xml = 1
    sub_mqtt = sub_mqtt_1;//select subscription 1
    menu_topic = menu_topic_2;
    header();
    tft.setCursor(5, 120);
    tft.setTextSize(2);
    tft.print("waiting for next message");
    footer();
    delay(1500);
    //tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    //tft.fillScreen(TFT_BLACK);
    //setup_wifi();
    //client.setServer(mqtt_server, 1883);
    client.subscribe(sub_mqtt);
    //
  }
  //
  // press up joypad for domoticz
  if ( digitalRead(WIO_5S_DOWN) == LOW) {//up button switch to domoticz
    alarm(1);
    tft.fillScreen(TFT_BLACK);
    client.unsubscribe(sub_mqtt);
    mqtt_conn = 2; //change to topic 2
    mqtt_json = 3; //if output is json/xml = 1
    sub_mqtt = sub_mqtt_2;//select subscription 2
    menu_topic = menu_topic_3;
    header();
    tft.setCursor(5, 120);
    tft.setTextSize(2);
    tft.print("waiting for next message");
    footer();
    delay(1500);
    //tft.fillRect(0, 30, 320, 180, TFT_BLACK);//x,y w,h color
    //tft.fillScreen(TFT_BLACK);
    //setup_wifi();
    //client.setServer(mqtt_server, 1883);
    client.subscribe(sub_mqtt);
    //
  }
  //
  //

  //Serial.println(timeClient.getFormattedTime());
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
