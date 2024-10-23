#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

//set mqtt server address to IP address [1:Server]
#define mqtt_server "192.168.68.104"

//define topic for publishing message to mqtt server
#define topic "sensor/photoresistor"

//create photoresistor sensor variables
int sensPin = 34;
int light;

//publish task activation counter
int count = 1;

//set up task handler
TaskHandle_t reading;
TaskHandle_t publish;

//set up WiFi
const char* SSID = "ilfandkym";
const char* passwd = "94263770";

//set up client [2:Client]
WiFiClient espClient;
PubSubClient client(espClient);

//set up tasks(a: Reading sensor value, b: Publishing sensor value to MQTT)
void readSensor(void* parameter)
{
  while(1)
  {
    if (count == 1)
    {
      vTaskResume(publish);
      count++;
    }
    light =  analogRead(sensPin);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.print("High water mark for reading: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
  }
}

void publishReading(void* parameter)
{
  while(1)
  {
    client.loop();
    client.publish(topic, String(light).c_str());
    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.print("High water mark for publish: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
  }
}


void setup() {

Serial.begin(9600);
WiFi.begin(SSID, passwd);
Serial.print("Connecting to WiFi...");

//configure sensor
pinMode(sensPin, INPUT);

//set address of server to be connected by client [3:Path of Connection between server and client]
client.setServer(mqtt_server, 1883);  

//checking WiFi connection
while (WiFi.status() != WL_CONNECTED) 
  {
  Serial.print(".");
  delay(100);
  } 
Serial.println("");
Serial.println("Connected to WiFi.");

//create tasks and suspend the tasks for now
xTaskCreatePinnedToCore(readSensor,"Read", 1300, NULL, 2, &reading, 0);
vTaskSuspend(reading);
xTaskCreatePinnedToCore(publishReading, "Publish", 3000, NULL,2, &publish,0);
vTaskSuspend(publish);

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
  //resume the tasks
  vTaskResume(reading);

}

