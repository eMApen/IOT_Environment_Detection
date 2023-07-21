#include "time_catch.h"

void Time_Catcher(struct tm *tm_struct){
  if(!getLocalTime(tm_struct))
   {
    Serial.println("Failed to obtain time");
    return;
  }
}
