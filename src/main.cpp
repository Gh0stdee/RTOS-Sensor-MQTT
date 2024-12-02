#include "RTOS.h"
#include "task.h"

void setup() {

//Serial Communication
Serial.begin(9600);

//configure sensor
pinMode(sensPin, INPUT);
delay(5000);

//configure MQTT and SD 
SDinit();
delay(1000);
serverSetUp();
configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);
delay(1000);
newFileCreate();


//create tasks and suspend the tasks for now
xTaskCreatePinnedToCore(readSensor,"Read", 8192, NULL, 1, &reading, 0);
vTaskSuspend(reading);
xTaskCreatePinnedToCore(publishReading, "Publish", 4096, NULL, 1, &publish,0);
vTaskSuspend(publish);


//Deep Sleep Modes
//Pin Voltage
pinMode(wakePin,INPUT_PULLDOWN);
pinMode(LED_BUILTIN,OUTPUT);
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

