



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

2.4 Components Justification

Development Process ............................................... 3

3.1 Timeline

3.2 Main Challenges & Solutions

Results ........................................................................ 3

Conclusion ................................................................. 3

Deliverables Submitted ............................................ 3

References ................................................................. 3













Executive Summary 
What we have done is that our team successfully designed a functional, low-cost IoT climate monitor for our library. This was done using the ESP32, BME280 sensor, 128×64 OLED display, and an RGB LED. We fulfilled all the core requirements: continuous temperature/humidity/pressure measurement through the integration of sensors, local display, web Interface, and visual alerts:
Individual Contributions (graded individually)
•	Kaeo Kloss: 

Wi-Fi connection, implementation of web server, debugging and optimization of codes, management of GitHub repositories, writing and formatting the full report, preparation of presentation, Hardware design and wiring
•	Antonio Lastoria: 


Technical Documentation
Software Architecture Single Arduino sketch with non-blocking loop. Key modules:
•	I^2C BME280 sensor reading
•	Rendering of OLED, Current Value, Trend Graphs
•	Built-in HTTP web server (auto-refresh every 5 s) and a graph
•	Threshold-based RGB LED alerts
Data Flow BME280>ESP32>real-time processing>LED alert>displayed locally + served over Wi-Fi

Hardware Connections
•	BME280: I^2C (SDA=GPIO32, SCL=GPIO33)
•	OLED (SPI): MOSI=23, SCK=18, DC=2, CS=17, RST=4
•	RGB LED: Red=GPIO14, Green=GPIO27, Blue=GPIO26 (common cathode and resistor)
•	Power: All devices from ESP32 3.3 V rail
Components Justification Total cost is less than $15 and all 3.3 V compatible, low power usage, and easy library support.

Development Process
Timeline 
Week 1: Component testing & wiring, Sensor + OLED + alerts working + Graphing + Web server + full integration
Week 2: Final debugging, report, demo

Main Challenges & Solutions
• OLED not initializing because of wrong SPI pins; solved by correcting pinout.
• BME280 detection problems. Solved by soldering pins

Results
•	True to life, real-time readings
•	Smooth temperature & humidity trend graphs
•	Instant red/green LED alerts
•	Responsive web dashboard accessible on local network (

Conclusion
This system is a clean, simple, low-cost solution for climate monitoring, fulfilling all the features required and the enhanced ones. This project is a great way to dive deeper into the subject of IOT and data collection while improving our group working skills/collaborative engineering.

Deliverables Submitted
•	Complete, fully commented source code (GitHub: https://github.com/Kaeokk/Smart-Indoor-Climate-Monitor-with-Local-Display)
•	Technical report
•	Successful 10-minute live demonstration
References
•	Adafruit BME280 & SSD1306 libraries
•	ESP32 Arduino Core documentation
•	Project GitHub repository

