#include "time.h"
#include <FS.h>
#include <SD.h>
#include <SPI.h>

//set up task handler
TaskHandle_t reading;
TaskHandle_t publish;

String dataMessage;

String getTimestamp()
{
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo))
    Serial.println("Failed to obtain local time");
  char Timeformat[50];                   //char array/string for storing the formatted date
  strftime(Timeformat, 50, "%d-%m-%G %T", &timeInfo);  //strfttime formatting
  String timeStamp = Timeformat;
  return timeStamp;
}

//set up tasks
void readSensor(void* parameter)
{
  while(1)
  {
    darkness =  analogRead(sensPin);
    //get timestamp and store value
    dataMessage = getTimestamp() + " " + String(darkness) + "\r\n";
    //append message to SD card

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