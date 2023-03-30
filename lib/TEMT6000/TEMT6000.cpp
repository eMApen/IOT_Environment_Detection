#include "TEMT6000.h"
#include <Wire.h>
#include <WiFi.h>
/// @brief  TEMT READING
/// @param signal ADC pin
/// @return The sensor value
int TEMT6000_read(uint8_t signal){
    int val;
    val=analogRead(signal);      //connect sensor to Analog 0
    //   Serial.println("ADC Sample Value:");
    //   Serial.println(val);    //print the value to serial
    return val;
}


/// @brief  TEMT8000 filtering
/// @param signal ADC pin
/// @return The sensor value
int TEMT6000_filter(){
    int i;
    int filter_sum = 0;
    for(i = 0; i < FILTER_N; i++) {
    filter_sum += TEMT6000_read(ad_pin);
    delay(1);
    }
    return (int)(filter_sum / FILTER_N);
}
