#include <Arduino.h>
#include <wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SSD1306.h>



Adafruit_BME280 bme;

int DIN = 18;
int DC = 2;
int CS = 17;
int CLK = 5;
int RES = 4;
int RedPin = 14;
int GreenPin = 27;
int BluePin = 26;
int BMESCL = 33;
int BMESDA = 32;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

Serial.println(F("SSD1306 allocation failed"));

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 10);
display.println("Hello, SSD1306!");
display.display();
}










void loop() {











}


