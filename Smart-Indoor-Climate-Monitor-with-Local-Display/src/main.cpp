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

// graph and numbers switch, start with show numbers
bool showNumbers = true;
//oled switching pin
int buttonPin = 13;

// Defining OLED pinout
int DIN = 23;
int DC = 2;
int CS = 17;
int CLK = 18;
int RS = 4;


//OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, DC, RS, CS);

//low power mode
unsigned long buttonPressStart = 0;
bool oledOff = false;



// Defining RGB pinout
int RedPin = 14;
int GreenPin = 27;

// Defining BME sensor pinout
int BMESCL = 33;
int BMESDA = 32;
Adafruit_BME280 bme;

// Thresholds  for alerts (starting)
float tempHigh = 28.0;
float humHigh = 60.0;

//Wifi credentials
const char* ssid = "Greg";
const char* password = "password1234";

// WEB SERVER
WebServer server(80);

#define HISTORY_SIZE 64
float tempHistory[HISTORY_SIZE];
float humHistory[HISTORY_SIZE];
int historyIndex = 0;

//set sensor read time to zero
unsigned long lastSample = 0;

//set up history
void addToHistory(float temp, float hum) {
  tempHistory[historyIndex] = temp;
  humHistory[historyIndex] = hum;
  historyIndex++;
  if (historyIndex >= HISTORY_SIZE) historyIndex = 0;
}

// Draw one history graph curve
void drawGraph(float *dataArray, float minVal, float maxVal) {
  for (int i = 1; i < HISTORY_SIZE; i++) {

    int x1 = (i - 1);
    int x2 = i;
    float v1 = dataArray[(historyIndex + i - 1) % HISTORY_SIZE];
    float v2 = dataArray[(historyIndex + i) % HISTORY_SIZE];
    int y1 = map(v1, minVal, maxVal, SCREEN_HEIGHT - 1, 10);
    int y2 = map(v2, minVal, maxVal, SCREEN_HEIGHT - 1, 10);

    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }
}



//function to switch oled
void updateOLED() {
display.clearDisplay();

 //numbers only
if (showNumbers) {
  // Display sensor readings in large numbers.
    display.setTextSize(2); // Set font size.
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0); // Set cursor for Temperature (T).
    display.print("T:");
    display.print(bme.readTemperature(), 1);
    display.println("C");

    display.setCursor(0, 25);
    display.print("H:");
    display.print(bme.readHumidity(), 1);
    display.println("%");

    display.setCursor(0, 50);
    display.print("P:");
    display.print((int)(bme.readPressure()/100)); // Read and print Pressure in hPa.
    display.println("hPa");
  } else {
    //graph
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Temp & Hum Trend");
    drawGraph(tempHistory, 10, 40); //call on drawGraph, using history array.
    drawGraph(humHistory,  20, 90); //call on drawGraph
  }
  //display either
  display.display();
}

void setup() { 
Serial.begin(9600);

//Leds
pinMode(RedPin, OUTPUT);
pinMode(GreenPin, OUTPUT);

  // OLED troubleshooting
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (1);
  }
  // Then BME280 troubleshooting
  Wire.begin(BMESDA, BMESCL);
  if (!bme.begin(0x76) && !bme.begin(0x77)) {
    Serial.println("BME280 not found");
  }

  //oled switcing button
  pinMode(buttonPin, INPUT_PULLUP);

//initialize wifi
WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());
  Serial.println("All devices working");

// WEB SERVER...Define the handler for the root path ("/") of the web server. This is the main page users see.
server.on("/", []() {
// Send an HTTP response with status 200 (OK) and content type "text/html".
  server.send(200, "text/html",
R"=====(<body>

T: <span id="t">??</span>&deg;C | H: <span id="h">??</span>%<br>

Temp: <input id="tl" size="2" value="28">
Hum:  <input id="hl" size="2" value="60">
<button onclick="fetch('/set?t='+tl.value+'&h='+hl.value)">SET</button>

<script>
setInterval(()=>{
  fetch('/data').then(r=>r.text()).then(x=>{
    [t,h] = x.split(',');
    t=document.getElementById('t'); h=document.getElementById('h');
    t.innerText = x.split(',')[0]; h.innerText = x.split(',')[1];
  });
},2000);
</script>

</body>)=====");
});
// This path only sends sensor data
  server.on("/data", [](){
    // Send a simple, plain text response back to the browser
    server.send(200, "text/plain", 
      // Reads the current Temperature and Humidity from the BME280 sensor as plain text
      String(bme.readTemperature(),1) + "," + String(bme.readHumidity(),1));
  });
//This link updates the alarm limits on the board
  server.on("/set", [](){
    // If the URL has a t value, update the codes tempHigh limit
    if(server.hasArg("t")) tempHigh = server.arg("t").toFloat();
    // If the URL has an h value, update the codes humHigh limit
    if(server.hasArg("h")) humHigh  = server.arg("h").toFloat();
    // Send a confirmation back to the browser
    server.send(200, "text:)", "OK");
  });
// Start the web server now, so it can listen for users
  server.begin();                     
  Serial.println("Web server running");  
  // show first screen 
  updateOLED();   
}

void loop() {
//call server
server.handleClient();

//READ VALUES ONCE
float temp = bme.readTemperature(); 
float hum  = bme.readHumidity();
// Record sensor every .2 seconds
 if (millis() - lastSample > 200) {
  lastSample = millis();
  addToHistory(temp, hum);
  }

// led alerts
  if (temp > tempHigh || hum > humHigh) {
  digitalWrite(RedPin, HIGH);
  digitalWrite(GreenPin, LOW);
  } else {
  digitalWrite(RedPin, LOW);
  digitalWrite(GreenPin, HIGH); 
  }

//Button Handling
if (digitalRead(buttonPin) == LOW) {
if (buttonPressStart == 0) buttonPressStart = millis(); // Start timer on press
} else {
// Button released=check duration
if (buttonPressStart != 0) {
unsigned long pressDuration = millis() - buttonPressStart;
    
if (pressDuration > 2000) {
// LONG PRESS=Toggle OLED On/Off (Low Power Mode)
oledOff = !oledOff;
if (oledOff) {
display.clearDisplay();
display.display();
}
} else if (pressDuration > 50) { 
// SHORT PRESS=Toggle Numbers and Graph 
if (!oledOff) {
showNumbers = !showNumbers;
}
}
buttonPressStart = 0; // Reset timer
}
}
// Final OLED Update
if (!oledOff) {
updateOLED(); 
}
}