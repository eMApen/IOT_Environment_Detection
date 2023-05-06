// Optionally include custom images
#include <WiFiMulti.h>

#include <SPI.h>             // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Spi.h"

#include "spaceman.h"
#include "TEMT6000.h"

#include "time_catch.h"


// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
SSD1306Spi display(22U, 21U, 0U);  // RES, DC, CS

#define DEMO_DURATION 3000

char Date[11],Time[9],Light[11],TP[16];

int year,month,day,hour,minu,sec;

int light_value, light_raw;

int watch_image_current = 0;   // save now display image number

WiFiMulti wifiMulti;


void pattern_main();