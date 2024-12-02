#include <Arduino.h>
#include "variables.h"

//indicate wake up state        
void blink()                                           
{
  digitalWrite(LED_BUILTIN,1); 
  delay(500);                                                //builtin LED blink for half a second
  digitalWrite(LED_BUILTIN,0);
}


void powerDown()
{
  if (espState)  //if ESP is "off" (i.e. wake up due to timer), no need to run the state change code
  {
    Serial.println("Going to sleep...");
    espState = false;
  }
  esp_deep_sleep_start();
}

void powerUp()
{
  espState = true;
  Serial.println("Waking up");
  blink();                          
}

void lightCheck()
{
    if (voltageWake && !espState)
    {
        if (voltageWake_count == 1) 
        {
        blink(); 
        espState = true;
        voltageWake_count +=1;
        }
        else
        {
            //nothing happens
        }
    }

    else if (darkness > 11)                             
    {
        if(voltageWake)
        {
        Serial.println("Please turn on the lights.");
        }
        else
        powerDown();
    }
    
    else if (darkness < 11 && !espState) 
    {
        powerUp();
    }
}