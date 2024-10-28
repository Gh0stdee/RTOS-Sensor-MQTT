#include <Arduino.h>
#include "mqtt_wifi.h"

//set up task handler
TaskHandle_t reading;
TaskHandle_t publish;

//create photoresistor sensor variables
int sensPin = 34;
int light;
//int counter =1;

//set up tasks(a: Reading sensor value, b: Publishing sensor value to MQTT)
void readSensor(void* parameter)
{
  while(1)
  {
    
    /* reset root cause
    if (counter == 1)
    {
        vTaskResume(publish);
        counter++;
    }
    */
    light =  analogRead(sensPin);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.print("High water mark for reading: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
    //Serial.println(counter); Debug Code
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