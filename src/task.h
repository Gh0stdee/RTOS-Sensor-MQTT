#include "SDcode.h"
#include "mqtt_wifi.h"
#include "deepSleep.h"

//set up task handler
TaskHandle_t reading;
TaskHandle_t publish;

//set up tasks

void readSensor(void* parameter)
{
  while(1)
  {
    //SD
    light =  analogRead(sensPin);
    memoryCheck();
    dataMessage = getTimestamp() + " " + String(light) + "\r\n";
    appendFile(SD,"/lightData.txt",dataMessage.c_str());

    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.print("High water mark for reading: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    //Deep Sleep
    darkness = map(light, 0, 4095, 0, 15);
    lightCheck();
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