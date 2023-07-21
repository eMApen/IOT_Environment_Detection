#include"display.h"
#include <WiFiMulti.h>

#define DEMO_DURATION 3000

// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
SSD1306Spi display(22U, 21U, 0U);  // RES, DC, CS

char Date[11],Time[9],Light[11],TP[16];

int year,month,day,hour,minu,sec;

int watch_image_current = 0;   // save now display image number


/// @brief Display functions
/// @param picture_num 
void drawImageDemo_94_64_w(int picture_num) {
  if(picture_num <0 || picture_num > 45)
  {
    Serial.println("error picture_num");
    picture_num = 0;
  }  

//  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, watch_image_addr[picture_num]);
// （128-94）*0.5 = 17 居中显示
  display.drawXbm(54, 0, Watch_Image_width, Watch_Image_height, spaceman[picture_num]);

}

void value2String(struct tm *timep,int light_value,int online){
  year = timep->tm_year + 1900;    // 年份从1900开始
  month = timep->tm_mon + 1;    // 0表示1月
  day = timep->tm_mday;
  sprintf(Date,"%d-%02d-%02d",year,month,day);
  sprintf(Time,"%02d:%02d:%02d",timep->tm_hour,timep->tm_min,timep->tm_sec);
  sprintf(Light,"Light=%d ",light_value);
  Light[8] = '%';
  //sprintf(TP,"T=%.0f°C P=%d",36.0,100);
  if (online != WL_CONNECTED)
  {
    /* code */
    sprintf(TP,"Not Online.");
    Serial.println("Wifi connection lost");
  }
  else{
    sprintf(TP,"Love, Life!");
  }
  
}



void display_bmp_watch(int current,struct tm *timep,int light_value,int online){
  // 1. display
  // clear the display
  display.clear();
  // draw the current demo method
  // demos[watch_image_current]();
  drawImageDemo_94_64_w(current);

  display.setFont(ArialMT_Plain_14);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  value2String(timep,light_value,online);
  display.drawString(0, 0, Date);
  display.drawString(0, 16, Time);
  display.drawString(0, 32, Light);
  display.drawString(0, 48, TP);
//  write the buffer to the display
  display.display();
}

//enter pattern1(weather/time) ---- 
void pattern_main(struct tm *timep,int light_value,int online)
{
  //display watch image
  if (watch_image_current < WATCH_IMAGE_NUM -1 )
    watch_image_current++;
  else 
    watch_image_current=0;

  display_bmp_watch(watch_image_current,timep,light_value,online);
  //get weather/time

}

void OLED_init(){
  display.init();
  display.setFont(ArialMT_Plain_10);
}
