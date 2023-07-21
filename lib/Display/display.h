// Optionally include custom images
#include <SPI.h>             // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Spi.h"
#include "spaceman.h"



void OLED_init();
void pattern_main(struct tm *timep,int light_value,int online);