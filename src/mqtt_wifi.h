#include <PubSubClient.h>
#include <WiFi.h>
#include "variables.h"

//set mqtt server address to IP address [1:Server]
#define mqtt_server "192.168.68.101"

//define topic for publishing message to mqtt server
#define topic "sensor/photoresistor"

//set up WiFi
const char* SSID = "ilfandkym";
const char* passwd = "94263770";

//set up client 
WiFiClient espClient;
PubSubClient client(espClient);

//set up connection/reconnection function for mqtt server
void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("Sensor")) //attempts connection with server: return true(succeed)/false(fail) [4: Connection through path]
    {
      Serial.println("Connected");
    }
    else 
    {
      Serial.println("Reconnecion failed. Trying again in 5 seconds.");
      delay(5000);
    }

  }
}

//WiFi, NTP & MQTT server set-up
void serverSetUp() 
{
  //set address of MQTT server
  client.setServer(mqtt_server, 1883);
  //client.setBufferSize(1024);  

  //WiFi and NTP server connection
  WiFi.begin(SSID, passwd);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) 
    {
    Serial.print(".");
    delay(500);
    } 
  Serial.println("");
  Serial.println("Connected to WiFi.");
  configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);
}