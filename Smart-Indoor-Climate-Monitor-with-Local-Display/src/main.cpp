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
int DIN = 23;
int DC = 2;
int CS = 17;
int CLK = 18;
int RS = 4;

//OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, DC, RS, CS);


// Defining RGB pinout
int RedPin = 14;
int GreenPin = 27;
int BluePin = 26;

// Defining BME sensor pinout
int BMESCL = 33;
int BMESDA = 32;

// BME280
// #define BME280_ADDRESS 0x76
Adafruit_BME280 bme;

// Thresholds  for alerts
float tempHigh = 28.0;
float humHigh = 60.0;

// WEB SERVER
WebServer server(80);

#define HISTORY_SIZE 128
float tempHistory[HISTORY_SIZE];
float humHistory[HISTORY_SIZE];
int historyIndex = 0;

unsigned long lastSample = 0;

void addToHistory(float temp, float hum) {
  tempHistory[historyIndex] = temp;
  humHistory[historyIndex] = hum;

  historyIndex++;
  if (historyIndex >= HISTORY_SIZE) historyIndex = 0;
}

// Draw one history graph curve

void drawGraph(float *dataArray, float minVal, float maxVal) {
  for (int i = 1; i < HISTORY_SIZE; i++) {

    int x1 = i - 1;
    int x2 = i;

    float v1 = dataArray[(historyIndex + i - 1) % HISTORY_SIZE];
    float v2 = dataArray[(historyIndex + i) % HISTORY_SIZE];

    int y1 = map(v1, minVal, maxVal, SCREEN_HEIGHT - 1, 10);
    int y2 = map(v2, minVal, maxVal, SCREEN_HEIGHT - 1, 10);

    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }
}

// OLED: Draw temperature + humidity historical graph
void showGraphs() {
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp/Humidity Trends");

  // Temperature (upper line)
  drawGraph(tempHistory, 15, 35);

  // Humidity (lower line)
  drawGraph(humHistory, 30, 90);

  display.display();
}

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
                  "Temp: " + String(t,1) + " &deg;C<br>"
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
  // Record sensor every 2 seconds
  if (millis() - lastSample > 2000) {
    lastSample = millis();
    float t = bme.readTemperature();
    float h = bme.readHumidity();
    addToHistory(t, h);   // I and H didnt exist
  }

  // Show graph on OLED
  showGraphs();
  
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


