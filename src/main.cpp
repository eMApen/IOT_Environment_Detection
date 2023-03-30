// Include the correct display library

// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include <WiFi.h>

// For a connection via SPI include:
#include <SPI.h>             // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Spi.h"

// Optionally include custom images
#include "images.h"
#include "spaceman.h"
#include "TEMT6000.h"


// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
SSD1306Spi display(22U, 21U, 0U);  // RES, DC, CS


#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;
int light_value;
int watch_image_current = 0;   // save now display image number
char Date[9],Time[9],Light[11],TP[16];



void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!");
  Serial.println();
  

  // Initialising the UI will init the display too.
  display.init();

  display.setFont(ArialMT_Plain_10);


}



void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

void drawImageDemo() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

Demo demos[] = {drawProgressBarDemo, drawImageDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

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

void value2String(){
  sprintf(Date,"23-03-30");
  sprintf(Time,"21:57:20");
  sprintf(Light,"Light=%d ",100*light_value/4096);
  Light[8] = '%';
  sprintf(TP,"T=%.0f°C P=%d",36.0,100);
}



void display_bmp_watch(int current){
  // 1. display
  // clear the display
  display.clear();
  // draw the current demo method
  // demos[watch_image_current]();
  drawImageDemo_94_64_w(current);

  display.setFont(ArialMT_Plain_14);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  value2String();
  display.drawString(0, 0, Date);
  display.drawString(0, 16, Time);
  display.drawString(0, 32, Light);
  display.drawString(0, 48, TP);
//  write the buffer to the display
  display.display();
}

//enter pattern1(weather/time) ---- 
void pattern_main()
{
  //display watch image
  if (watch_image_current < WATCH_IMAGE_NUM -1 )
    watch_image_current++;
  else 
    watch_image_current=0;

  display_bmp_watch(watch_image_current);
  //get weather/time

}

void loop() {
  // clear the display
  display.clear();

  light_value = TEMT6000_filter();
  Serial.println(light_value);
  pattern_main();
  counter++;
  
  delay(50);
}

