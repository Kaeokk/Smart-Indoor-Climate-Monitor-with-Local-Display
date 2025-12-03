



Internet of Things 1
Problem-Based Learning Unit



Presented to Jimmy Plaitis


Submitted by Kaeo Kloss and Antonio Lastoria (sec. 00002) on November 28th 2025














Table of Contents

Executive Summary ...................................................... 1
Technical Documentation ......................................... 2
2.1 Software Architecture
2.2 Data Flow
2.3 Hardware Connections
2.4 Circuit Schematic
2.5 Components Justification
Development Process ............................................... 3
3.1 Timeline
3.2 Main Challenges & Solutions
Results ........................................................................ 4
Conclusion ................................................................ 5
Deliverables Submitted ............................................ 6
References ................................................................. 7
Source Code ................................................................. 8

















1. Executive Summary 
Our team successfully designed a functional, low-cost IoT climate monitor for our library. This was done using the ESP32, BME280 sensor, 128×64 OLED display, and an RGB LED. We fulfilled all the core requirements: continuous temperature/humidity/pressure measurement through the integration of sensors, local display, web Interface, and visual alerts to provide continuous monitoring of temperature, humidity and pressure. 

In addition to those requirements, we have
•	Threshold-based alerts with customizable limits via the web interface
•	Graphical display of historical trends on the OLED
•	Configuration mode for threshold adjustment through the web interface
•	Low-power operation with sleep mode
________________________________________

Individual Contributions (graded individually)
•	Kaeo Kloss: 
Wi-Fi connection, implementation of web server, debugging and optimization of codes, management of GitHub repositories, writing and formatting the full report, preparation of presentation, Hardware design and wiring, low power mode.
•	Antonio Lastoria: 
Worked on the baseline of the code, Writing and formatting the full report, worked on the JSON file for the code. Managing the GitHub page.

For more history see commits on GitHub

2. Technical Documentation
 2.1 Software Architecture Single Arduino sketch with non-blocking loop.  
Key modules:
•	I^2C BME280 sensor reading
•	Rendering of OLED, Current Value, Trend Graphs
•	Built-in HTTP web server (auto-refresh every 5 s) with values and variable limit alerts
•	Threshold-based RGB LED alerts
•	A button to set the graph to a display of values or a graph, and to put into…
•	Low power mode with OLED off

 
Figure 1 The web UI for the ESP32 temperature monitor sensor showing the temperature and humidity values with spaces to set limits for alerts.


  
Figure 2 The OLED display showing live temperature readings and a graph of humidity and temperature



 2.2 Data Flow BME280>ESP32>real-time processing>LED alert>displayed locally + served over Wi-Fi
 2.3 Hardware Connections
Device 	Connections 	Notes
BME280 SDA	GPIO 32	Used for I2C data
BME280 SCL	GPIO 33	Used for I2C clock
OLED MOSI	GPIO 23	Used for SPI data
OLED SCK	GPIO 18	Used for SPI clock
OLED DC	GPIO 2	Data/Command
OLED CS	GPIO 17	Chip Select
OLED RST	GPIO 4	Reset
RGB Red	GPIO 14	PWM capable
RGB Green	GPIO 27	PWM capable
RGB Blue	GPIO 26	PWM capable

2.4 Circuit schematic

 
Figure 3 The circuit schematic for the indoor climate monitoring system 
 
Figure 4 An example complete build of the circuit. 



 

2.5 Components Justification 
Item	Qty	Unit Price
ESP32-Devkit	1	 $1.63
BME280 module	1	  $1.77
0.96 64x128 SPI OLED SSD1306	1	1.39
RGB module	1	$0.01
Wires 	1	$1.63
Button	1	$0.16




Total cost is less than $15 and all 3.3 V compatible, low power usage, and easy library support. An overall efficient and cheap project. 
3. Development Process
 3.1 Timeline 
Week 1: Component testing & wiring, Sensor + OLED + alerts working + Graphing + Web server + full integration
Week 2: Troubleshooting and debugging our circuit, report, low power mode
Week 3: Final debugging and demo of the circuit
 3.2 Main Challenges & Solutions
• OLED not initializing because of wrong SPI pins; solved by correcting pinout.
• BME280 detection problems. Solved by soldering pins
4. Results
•	True to life, real-time readings
•	Smooth temperature & humidity trend graphs
•	Instant red/green LED alerts
•	Responsive web dashboard accessible on local network 
•	OLED with graphs, values and a sleep mode


5. Conclusion
This system is a clean, simple, low-cost solution for climate monitoring, fulfilling all the features required and the enhanced ones. This project is a great way to dive deeper into the subject of IOT and data collection while improving our group working skills/collaborative engineering. The IOT circuit meets the core requirements with continuous measurements of temperature, humidity, and pressure. 
6. Deliverables Submitted
•	Complete, fully commented source code GitHub or below: https://github.com/Kaeokk/Smart-Indoor-Climate-Monitor-with-Local-Display)
•	Technical report
•	10-minute live demonstration
7. References
• Adafruit BME280 & SSD1306 libraries
adafruit/Adafruit_BME280_Library: Arduino Library for BME280 sensors
adafruit/Adafruit_SSD1306: Arduino library for SSD1306 monochrome 128x64 and 128x32 OLEDs

• ESP32 Arduino Core documentation
https://www.espressif.com/en/support/documents/technical-documents

• Project GitHub repository
https://github.com/Kaeokk/Smart-Indoor-Climate-Monitor-with-Local-Display
              •  Web search and AI

Prices:
• https://www.aliexpress.com/w/wholesale-Full%2525252dcolor-LED-Diode-RGB-.html?spm=a2g0o.productlist.search.0
• https://www.aliexpress.com/w/wholesale-Wires-dupont.html?spm=a2g0o.productlist.search.0
•  https://www.aliexpress.com/w/wholesale-bme-280.html?spm=a2g0o.productlist.search.0
•  https://www.aliexpress.com/w/wholesale-button-component.html?spm=a2g0o.productlist.search.0
•  https://www.aliexpress.com/w/wholesale-esp32.html?spm=a2g0o.productlist.search.0
•  https://www.aliexpress.com/w/wholesale-0.96-64x128-SPI-OLED-SSD1306.html?spm=a2g0o.productlist.search.0




8. Source code:
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
// Button released: check duration
if (buttonPressStart != 0) {
unsigned long pressDuration = millis() - buttonPressStart;
    
if (pressDuration > 2000) {
// LONG PRESS: Toggle OLED On/Off (Low Power Mode)
oledOff = !oledOff;
if (oledOff) {
display.clearDisplay();
display.display();
}
} else if (pressDuration > 50) { 
// SHORT PRESS: Toggle Numbers/Graph
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

