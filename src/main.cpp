#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include <password.h>
#include <WiFiMulti.h>


// For a connection via SPI include:
#include <SPI.h>             // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Spi.h"

// Optionally include custom images
#include "spaceman.h"
#include "TEMT6000.h"


#define DEVICE "ESP32"
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <time.h>



// For InfluxDB Data Update 
WiFiMulti wifiMulti;

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
Point sensor("Light");

// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
SSD1306Spi display(22U, 21U, 0U);  // RES, DC, CS

#define DEMO_DURATION 3000

typedef void (*Demo)(void);
void Time_Catcher();

int demoMode = 0;
int counter = 1;
int light_value, light_raw;
float light_update=0;
int watch_image_current = 0;   // save now display image number
char Date[11],Time[9],Light[11],TP[16];
struct tm timeinfo;
int year,month,day,hour,minu,sec;
uint32_t updateflag =0;


void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!");
  Serial.println();
  
  display.init();


  // WiFi and Database Initialize Setting
  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Initialising the UI will init the display too.
  
  // Add tags
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());
  
  display.setFont(ArialMT_Plain_10);

  configTime(60*60*8, 0, "ntp3.aliyun.com");  //配置时钟服务器
  Time_Catcher();

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } 
  else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

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

void value2String(){
  Time_Catcher();
  year = timeinfo.tm_year + 1900;    // 年份从1900开始
  month = timeinfo.tm_mon + 1;    // 0表示1月
  day = timeinfo.tm_mday;
  sprintf(Date,"%d-%02d-%02d",year,month,day);
  sprintf(Time,"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
  sprintf(Light,"Light=%d ",light_value);
  Light[8] = '%';
  //sprintf(TP,"T=%.0f°C P=%d",36.0,100);
  if (wifiMulti.run() != WL_CONNECTED)
  {
    /* code */
    sprintf(TP,"Not Online.");
  }
  else{
    sprintf(TP,"Love, Life!");
  }
  
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

void Time_Catcher(){
  if(!getLocalTime(&timeinfo))
   {
    Serial.println("Failed to obtain time");
    return;
  }
}


void Influxdb_Writting(){
  //10s左右上传一次数据
  light_update = light_update + (float)light_value;
  if(updateflag == 200)
  {
    updateflag = 0;
    sensor.clearFields();
    // Store measured value into point
    // Report RSSI of currently connected network
    sensor.addField("rssi", WiFi.RSSI());
    light_update = light_update/200;
    sensor.addField("light",light_update);
    // Print what are we exactly writing
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());

    // Check WiFi connection and reconnect if needed
    if (wifiMulti.run() != WL_CONNECTED) {
      Serial.println("Wifi connection lost");
    }

    // Write point
    if (!client.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    }

    Serial.println("Wait 10s");
    light_update = 0;
  }
  else{
    updateflag++;
  }
}


/// @brief LOOP----------------------
void loop() {

  (light_raw,light_value) = TEMT6000_Output();
  pattern_main();
  Influxdb_Writting();
  delay(5);
}



