#include <main.h>
void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!");
  Serial.println();
  
  OLED_init();
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
  configTime(60*60*8, 0, "ntp3.aliyun.com");  //配置时钟服务器
  InfluxDB_Init();
}



/// @brief LOOP----------------------
void loop() {
  online = wifiMulti.run();
  Time_Catcher(&timeinfo);// Update struct timeinfo
  (light_raw,light_value) = TEMT6000_Output();
  pattern_main(&timeinfo,light_value,online);
  Influxdb_Writting(light_value);
  delay(5);
}



