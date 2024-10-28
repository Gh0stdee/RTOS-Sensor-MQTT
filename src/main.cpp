#include "task.h"

void setup() {

Serial.begin(9600);
WiFi.begin(SSID, passwd);
Serial.print("Connecting to WiFi...");

//configure sensor
pinMode(sensPin, INPUT);

//set address of server to be connected by client [3:Path of Connection between server and client]
client.setServer(mqtt_server, 1883);
client.setBufferSize(1024);  

//checking WiFi connection
while (WiFi.status() != WL_CONNECTED) 
  {
  Serial.print(".");
  delay(100);
  } 
Serial.println("");
Serial.println("Connected to WiFi.");

//create tasks and suspend the tasks for now
xTaskCreatePinnedToCore(readSensor,"Read", 2048, NULL, 1, &reading, 0);
vTaskSuspend(reading);
xTaskCreatePinnedToCore(publishReading, "Publish", 4096, NULL, 1, &publish,0);
vTaskSuspend(publish);

}

void loop() {
  //connect client to MQTT
  if (!client.connected())
    reconnect();
  vTaskResume(reading);
  vTaskResume(publish);
  vTaskDelete(NULL);
}

