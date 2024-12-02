#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "time.h"
#include "pinout.h"
#include <ArduinoJson.h>
#include <Arduino.h>

int size_1mb = 1048576;

String dataMessage;
DynamicJsonDocument doc (2*size_1mb); 

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

void writeFile(fs::FS &fs, const char* path, const char* message)
{
    Serial.println("Writing file.....");
    File data = fs.open(path,FILE_WRITE);
    if(data.print(message))
    {
        Serial.println("Data written.");
    }
    else
    {
        Serial.println("Writing failed.");
    }
    data.close();
}

//create file if file doesn't exist
void newFileCreate()
{
  File file = SD.open("/lightData.txt");                     //create a file on SD card and open the file
  if(!file)
  {
    writeFile(SD,"/lightData.txt","Timestamp           Light \r\n"); // \r\n means go to next line in the file
    Serial.println("Creating file...");
    file.close();                                      //close the file
  }
}

void appendFile(fs::FS &fs, const char* path, const char* message)
{
    Serial.println("Appending file.....");
    File data = fs.open(path,FILE_APPEND);
    if(data.print(message))
    {
        Serial.println("Message appended.");
    }
    else
    {
        Serial.println("Message not appended.");
    }
    data.close();
}

void deleteFile(fs::FS &fs, const char* path)
{
    if(fs.remove(path))
    {
        Serial.println("File deleted.");
    }
    else
    {
        Serial.println("Failed to delete file.");
    }
}

void memoryCheck()
{
  if (doc.memoryUsage()> size_1mb)
  {
    Serial.println("Exceeded 1mb");
    doc.clear();
    deleteFile(SD, "/lightData.txt");
    newFileCreate();
  }
}

void SDinit()
{
  SPI.begin(Sck,Miso,Mosi,csPin); //SCK,MISO,MOSI,cs pins
  while (!SD.begin(csPin))                                       
  {
    Serial.println("Error occurred at SD begin");
    delay(1000);
  }
}
