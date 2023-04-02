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
double TEMT6000_filter(){
    int i;
    int filter_sum = 0;
    for(i = 0; i < FILTER_N; i++) {
        filter_sum += TEMT6000_read(ad_pin);
    delay(1);
    }
    return ((double)filter_sum / FILTER_N);
}

/// @brief TEMT8000 使用A律十三折线，数据处理并且显示百分比
/// @param int signal: 平滑滤波器输出的信号
/// @return int Companded: 
int TEMT6000_Companding(double signal){
    Serial.println(signal);
    double lightpercent = (double)signal/4096;
    double Companded;
    
    if(lightpercent<=0.015625)
    {
        Companded = lightpercent*16;
    }
    else if (lightpercent>=0.015625&&lightpercent<=0.03125)
    {
        /* code */
        Companded = 0.25+(lightpercent-0.015625)*8;
    }
    else if (lightpercent>=0.03125&&lightpercent<=0.0625)
    {
        /* code */
        Companded = 0.375+(lightpercent-0.03125)*4;
    }
    else if (lightpercent>=0.0625&&lightpercent<=0.125)
    {
        /* code */
        Companded = 0.5+(lightpercent-0.0625)*2;
    }
    else if (lightpercent>=0.125&&lightpercent<=0.25)
    {
        /* code */
        Companded = 0.625+(lightpercent-0.125);
    }
    else if (lightpercent>=0.25&&lightpercent<=0.5)
    {
        /* code */
        Companded = 0.75+(lightpercent-0.25)*0.5;
    }
    else if (lightpercent>=0.5&&lightpercent<=1)
    {
        /* code */
        Companded = 0.875+(lightpercent-0.5)*0.25;
    }
    //Serial.println((int)(Companded*100));
    return (int)(Companded*100);
}


int TEMT6000_Output(){
    int lightpercent;
    double filtered;
    filtered = TEMT6000_filter();
    lightpercent = TEMT6000_Companding(filtered);
    //Serial.println(filtered);
    //Serial.println(lightpercent);
    return (filtered,lightpercent);
}