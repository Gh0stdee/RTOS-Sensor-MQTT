#include <Arduino.h>
#include "RTOS.h"
#include "task.h"

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


void setup() {

//Serial Communication
Serial.begin(9600);

//configure sensor
pinMode(sensPin, INPUT);

serverSetUp();

//create tasks and suspend the tasks for now
xTaskCreatePinnedToCore(readSensor,"Read", 2048, NULL, 1, &reading, 0);
vTaskSuspend(reading);
xTaskCreatePinnedToCore(publishReading, "Publish", 4096, NULL, 1, &publish,0);
vTaskSuspend(publish);

//set up timer to wake up and do lightlevel checking at a regular interval
esp_sleep_enable_timer_wakeup(detectGap * usFactor);

}

void loop() {
  //connect client to MQTT
  if (!client.connected())
    reconnect();
  vTaskResume(reading);
  vTaskResume(publish);
  vTaskDelete(NULL);
}

