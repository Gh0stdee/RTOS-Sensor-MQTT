#include <Arduino.h>
#include "RTOS.h"
#include "task.h"

void setup() {

//Serial Communication
Serial.begin(9600);

//configure sensor
pinMode(sensPin, INPUT);

//configure MQTT and SD 
serverSetUp();
newFileCreate();

//create tasks and suspend the tasks for now
xTaskCreatePinnedToCore(readSensor,"Read", 4096, NULL, 1, &reading, 0);
vTaskSuspend(reading);
xTaskCreatePinnedToCore(publishReading, "Publish", 4096, NULL, 1, &publish,0);
vTaskSuspend(publish);

//timer for deep sleep mode exit check 
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

