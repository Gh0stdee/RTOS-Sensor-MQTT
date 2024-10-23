#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

//set mqtt server address to IP address [1:Server]
#define mqtt_server "192.168.68.104"

//define topic for publishing message to mqtt server
#define topic "sensor/photoresistor"

//set up WiFi
const char* SSID = "ilfandkym";
const char* passwd = "98645270";

//set up client [2:Client]
WiFiClient espClient;
PubSubClient client(espClient);

//set up tasks(a: Reading sensor value, b: Publishing sensor value to MQTT)


void setup() {

Serial.begin(9600);
WiFi.begin(SSID, passwd);
Serial.print("Connecting to WiFi...");

//set address of server to be connected by client [3:Path of Connection between server and client]
client.setServer(mqtt_server, 1883);  

//checking WiFi connection
while (WiFi.status() != WL_CONNECTED) 
  {
  Serial.print(".");
  delay(50);
  } 
Serial.println("");
Serial.println("Connected to WiFi.");

//create threads
xTaskCreatePinnedToCore();
xTaskCreatePinnedToCore();

}

//set up connection/reconnection function for mqtt server
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
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

void loop() {
  //connect client to MQTT
  if (!client.connected())
    reconnect();
  


}

