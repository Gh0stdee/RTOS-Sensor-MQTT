#include "SDcode.h"
#include "mqtt_wifi.h"

//set up task handler
TaskHandle_t reading;
TaskHandle_t publish;

//set up tasks

void readSensor(void* parameter)
{
  while(1)
  {
    darkness =  analogRead(sensPin);



    
    //get timestamp and store value
    dataMessage = getTimestamp() + " " + String(darkness) + "\r\n";
    //append message to SD card
    appendFile(SD,"/lightData.txt",dataMessage.c_str());
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
    client.publish(topic, String(darkness).c_str());
    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.print("High water mark for publish: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
  }
}