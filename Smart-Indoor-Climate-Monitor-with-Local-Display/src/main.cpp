#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>


// Defining OLED pinout
int DIN = 18;
int DC = 2;
int CS = 17;
int CLK = 5;
int RS = 4;

// Defining RGB pinout
int RedPin = 14;
int GreenPin = 27;
int BluePin = 26;

// Defining BME sensor pinout
int BMESCL = 33;
int BMESDA = 32;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, DC, RS, CS);

#define BME280_ADDRESS 0x76
Adafruit_BME280 bme;

void setup() {
  Serial.begin(9600);

  // OLED first
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (1);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println("OLED OK");
  display.display();

  // Then BME280
  Wire.begin(32, 33);
  if (!bme.begin(0x76) && !bme.begin(0x77)) {
    Serial.println("BME280 not found");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sensor offline");
    display.display();
    return; // Don't halt the program
  }

  Serial.println("All devices initialized");
}


void loop() {




display.clearDisplay();
display.setTextSize(2);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0,0);
display.print("T:");
display.print(bme.readTemperature());
display.println("C");


display.print("H:");
display.print(bme.readHumidity());
display.println("%");

display.print("P:");
display.print(bme.readPressure());
display.println("Pa");
display.display();
}


