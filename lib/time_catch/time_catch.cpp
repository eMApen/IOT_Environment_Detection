#include "time_catch.h"

void Time_Catcher(){
  if(!getLocalTime(&timeinfo))
   {
    Serial.println("Failed to obtain time");
    return;
  }
}
