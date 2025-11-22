// Libraries for the data colection

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

// Libraries for the web server

#include <WiFi.h>
#include <WebServer.h>

//Wifi credentials
const char* ssid = "Greg";
const char* password = "password1234";

// Defining OLED pinout
int DIN = 18;
int DC = 2;
int CS = 17;
int CLK = 5;
int RS = 4;

//OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, DC, RS, CS, CLK);


// Defining RGB pinout
int RedPin = 14;
int GreenPin = 27;
int BluePin = 26;

// Defining BME sensor pinout
int BMESCL = 33;
int BMESDA = 32;

// BME280
#define BME280_ADDRESS 0x76
Adafruit_BME280 bme;

// Thresholds  for alerts
float tempHigh = 28.0;
float humHigh = 60.0;

// WEB SERVER
WebServer server(80);

void setup() {
Serial.begin(9600);
//Leds
pinMode(RedPin, OUTPUT);
pinMode(GreenPin, OUTPUT);
pinMode(BluePin, OUTPUT);

  // OLED 
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
  delay(1000);

  // Then BME280
  Wire.begin(BMESDA, BMESCL);
  if (!bme.begin(0x76) && !bme.begin(0x77)) {
    Serial.println("BME280 not found");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sensor offline");
    display.display();
    return; // Don't halt the program
  }
//initialize wifi
WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP Address: http:// ");
  Serial.println(WiFi.localIP());
  Serial.println("All devices working");

  // WEB SERVER ADDED — super simple page
  server.on("/", [](){
    float t = bme.readTemperature();
    float h = bme.readHumidity();
    float p = bme.readPressure() / 100.0F;
    
    String page = "<html><head><title>Room Monitor</title>"
                  "<meta http-equiv='refresh' content='5'></head>"  // refresh every 5 sec
                  "<body style='font-size:50px;text-align:center;margin-top:100px;'>"
                  "Temp: " + String(t,1) + " C<br>"
                  "Hum : " + String(h,1) + " %<br>"
                  "Pres: " + String(p,0) + " hPa"
                  "</body></html>";
    server.send(200, "text/html", page);
  });

  // WEB SERVER ADD HERE WIP

  server.begin();                     
  Serial.println("Web server running");  
}

void loop() {

//call server
server.handleClient();

//basic led alerts
  if (bme.readTemperature() > tempHigh || bme.readHumidity() > humHigh) {
    digitalWrite(RedPin, HIGH);
    digitalWrite(GreenPin, LOW);
  } else {
    digitalWrite(RedPin, LOW);
    digitalWrite(GreenPin, HIGH);
  }

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


