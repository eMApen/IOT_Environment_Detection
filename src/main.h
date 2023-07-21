#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include <password.h>
#include <WiFiMulti.h>

#include <time_catch.h>
#include <display.h>
#include <DBUpdate.h>
#include <TEMT6000.h>

#define DEVICE "ESP32"

int light_value, light_raw;
int counter = 1;
int online;



struct tm timeinfo;

WiFiMulti wifiMulti;

extern SSD1306Spi display;  // RES, DC, CS
