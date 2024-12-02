#include "RTOS.h"
#include "task.h"

void setup() {

//Serial Communication
Serial.begin(9600);

//configure sensor
pinMode(sensPin, INPUT);

//configure MQTT and SD 
serverSetUp();
configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);
newFileCreate();
SDinit();

//create tasks and suspend the tasks for now
xTaskCreatePinnedToCore(readSensor,"Read", 4096, NULL, 1, &reading, 0);
vTaskSuspend(reading);
xTaskCreatePinnedToCore(publishReading, "Publish", 4096, NULL, 1, &publish,0);
vTaskSuspend(publish);

//Deep Sleep Modes
//Pin Voltage
esp_sleep_enable_ext0_wakeup((gpio_num_t)wakePin,1);  
voltageWake = digitalRead(wakePin);
voltageWake_count = 1;

//Timer
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

