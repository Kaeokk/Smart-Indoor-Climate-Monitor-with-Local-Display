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

// graph and numbers switch
unsigned long lastSwitchTime = 0;
bool showNumbers = true;

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

//oled switching pin
int buttonPin = 13;



// BME280
// #define BME280_ADDRESS 0x76
Adafruit_BME280 bme;

// Thresholds  for alerts (starting)
float tempHigh = 28.0;
float humHigh = 60.0;

// WEB SERVER
WebServer server(80);

#define HISTORY_SIZE 64
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

  if (showNumbers) {
    //numbers only
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("T:");
    display.print(bme.readTemperature(), 1);
    display.println("C");

    display.setCursor(0, 25);
    display.print("H:");
    display.print(bme.readHumidity(), 1);
    display.println("%");

    display.setCursor(0, 50);
    display.print("P:");
    display.print((int)(bme.readPressure()/100));
    display.println("hPa");
  } else {
    //graph
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Temp & Hum Trend");

    drawGraph(tempHistory, 15, 35);
    drawGraph(humHistory,  30, 90);
  }

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

  //oled switcing button
  pinMode(buttonPin, INPUT_PULLUP);

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
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());
  Serial.println("All devices working");

  // WEB SERVER
server.on("/", []() {
  server.send(200, "text/html",
R"=====(<body>

T: <span id="t">??</span>deg&C | H: <span id="h">??</span>%<br>

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



  server.on("/data", [](){
    server.send(200, "text/plain", 
      String(bme.readTemperature(),1) + "," + String(bme.readHumidity(),1));
  });

  server.on("/set", [](){
    if(server.hasArg("t")) tempHigh = server.arg("t").toFloat();
    if(server.hasArg("h")) humHigh  = server.arg("h").toFloat();
    server.send(200, "text:)", "OK");
  });

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

//basic led alerts
  if (temp > tempHigh || hum > humHigh) {
    digitalWrite(RedPin, HIGH);
    digitalWrite(GreenPin, LOW);
  } else {
    digitalWrite(RedPin, LOW);
    digitalWrite(GreenPin, HIGH);
  }
  // Record sensor every .2 seconds
 if (millis() - lastSample > 200) {
    lastSample = millis();
    addToHistory(temp, hum);
  }

// BUTTON switching
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && buttonState == LOW) {  // falling edge
    showNumbers = !showNumbers;
  }

  lastButtonState = buttonState;

  // Update OLED (every render call)
  updateOLED();
}