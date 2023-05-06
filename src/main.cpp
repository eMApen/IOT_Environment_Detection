#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include <password.h>
#include <WiFiMulti.h>

#include <display.h>
#include <DBUpdate.h>

#define DEVICE "ESP32"

#include "time_catch.h"

int counter = 1;
int light_value;

float light_update=0;


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
  //
  display.setFont(ArialMT_Plain_10);
  configTime(60*60*8, 0, "ntp3.aliyun.com");  //配置时钟服务器
  Time_Catcher();// 获取时间
  InfluxDB_Init();
}



/// @brief LOOP----------------------
void loop() {

  (light_raw,light_value) = TEMT6000_Output();
  pattern_main();
  light_update=Influxdb_Writting(light_value);
  delay(5);
}



