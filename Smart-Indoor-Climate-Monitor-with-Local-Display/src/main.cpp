#include <Arduino.h>
#include <wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SSD1306.h>

#define Screen_Width 128
#define Screen_Height 64
#define OLED_MOSI 18
#define OLED_CLK 5
#define OLED_DC 2
#define OLED_CS 17
#define OLED_RES 4


#define Screen_Width 128
#define Screen_Height 64

_Adafruit_SSD1306_H_ display(Screen_Width, Screen_Height, &Wire, -1);
Adafruit_BME280 bme;







void setup() {

}








void loop() {


}


