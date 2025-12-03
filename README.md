



Internet of Things 1
Problem-Based Learning Unit



Presented to Jimmy Plaitis


Submitted by Kaeo Kloss and Antonio Lastoria (sec. 00002) on November 28th 2025














Table of Contents

Executive Summary ...................................................... 2
Technical Documentation ......................................... 2
2.1 Software Architecture
2.2 Data Flow
2.3 Hardware Connections
2.4 Circuit Schematic
2.5Components Justification
Development Process ............................................... 3
3.1 Timeline
3.2 Main Challenges & Solutions
Results ........................................................................ 3
Conclusion ................................................................. 3
Deliverables Submitted ............................................ 3
References ................................................................. 3
















1. Executive Summary 
Our team successfully designed a functional, low-cost IoT climate monitor for our library. This was done using the ESP32, BME280 sensor, 128×64 OLED display, and an RGB LED. We fulfilled all the core requirements: continuous temperature/humidity/pressure measurement through the integration of sensors, local display, web Interface, and visual alerts to provide continuous monitoring of temperature, humidity and pressure. 
Individual Contributions (graded individually)
•	Kaeo Kloss: 

Wi-Fi connection, implementation of web server, debugging and optimization of codes, management of GitHub repositories, writing and formatting the full report, preparation of presentation, Hardware design and wiring
•	Antonio Lastoria: 

Worked on the baseline of the code, Writing and formatting the full report, worked on the JSON file for the code. Managing the GitHub page 

2. Technical Documentation
 2.1 Software Architecture Single Arduino sketch with non-blocking loop.  
Key modules:
•	I^2C BME280 sensor reading
•	Rendering of OLED, Current Value, Trend Graphs
•	Built-in HTTP web server (auto-refresh every 5 s) and a graph
•	Threshold-based RGB LED alerts
 
Figure 1 The web UI for the ESP32 temperature monitor sensor showing the temperature and humidity graphs.
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

 
Figure 2 The circuit schematic for the indoor climate monitoring system 




 

2.5 Components Justification 
Item	Qty	Unit Price
ESP32-Devkit	1	 $6
BME280 module	1	  $1.77
0.96 64x128 SPI OLED SSD1306	1	$3.56
RGB module	1	$0.30
Wires 	1	$0.30




Total cost is less than $15 and all 3.3 V compatible, low power usage, and easy library support. An overall efficient and cheap project. 
3. Development Process
 3.1 Timeline 
Week 1: Component testing & wiring, Sensor + OLED + alerts working + Graphing + Web server + full integration
Week 2: Troubleshooting and debugging our circuit, report 
Week 3: Final debugging and demo of the circuit
 3.2 Main Challenges & Solutions
• OLED not initializing because of wrong SPI pins; solved by correcting pinout.
• BME280 detection problems. Solved by soldering pins
4. Results
•	True to life, real-time readings
•	Smooth temperature & humidity trend graphs
•	Instant red/green LED alerts
•	Responsive web dashboard accessible on local network (


5. Conclusion
This system is a clean, simple, low-cost solution for climate monitoring, fulfilling all the features required and the enhanced ones. This project is a great way to dive deeper into the subject of IOT and data collection while improving our group working skills/collaborative engineering. The IOT circuit meets the core requirements with continuous measurements of temperature, humidity, and pressure. 
Deliverables Submitted
•	Complete, fully commented source code (GitHub: https://github.com/Kaeokk/Smart-Indoor-Climate-Monitor-with-Local-Display)
•	Technical report
•	Successful 10-minute live demonstration
References
•	Adafruit BME280 & SSD1306 libraries
adafruit/Adafruit_BME280_Library: Arduino Library for BME280 sensors
adafruit/Adafruit_SSD1306: Arduino library for SSD1306 monochrome 128x64 and 128x32 OLEDs

•	ESP32 Arduino Core documentation
https://www.espressif.com/en/support/documents/technical-documents

•	Project GitHub repository
https://github.com/Kaeokk/Smart-Indoor-Climate-Monitor-with-Local-Display

