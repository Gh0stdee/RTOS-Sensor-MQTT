#include <PubSubClient.h>
#include <WiFi.h>

//set mqtt server address to IP address [1:Server]
#define mqtt_server "192.168.68.101"

//define topic for publishing message to mqtt server
#define topic "sensor/photoresistor"

//set up WiFi
const char* SSID = "ilfandkym";
const char* passwd = "94263770";

//set up client [2:Client]
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
