Smart Indoor Climate Monitor
ESP32 + BME280 + OLED + RGB LED + Wi-Fi Web Dashboard
College Ventilation Monitoring System

Project Overview
This IoT device continuously monitors temperature, humidity, and air pressure in library study rooms using a BME280 sensor. It provides:

Real-time data on a 128×64 OLED display
Visual alerts via RGB LED (Green = OK, Red = too hot/humid)
Remote monitoring through a simple built-in web dashboard (accessible from any phone/laptop on the same Wi-Fi)

Perfect for detecting stuffy, uncomfortable study rooms!
Features Implemented
Core Requirements (All Completed)

BME280 sensor integration (I2C)
Live OLED display with clear readings
Built-in web server (no internet required)
RGB LED visual warnings

Enhanced Features (Chosen & Completed)

Threshold-based alerts (28°C / 60% RH default)
Clean, responsive web interface with auto-refresh
Pressure shown in hPa (not raw Pa)

Hardware Required

ESP32 Development Board
BME280 Temperature/Humidity/Pressure Sensor
0.96" SSD1306 OLED Display (128×64, SPI mode)
Common Anode or Cathode RGB LED + 220Ω resistors
Breadboard & jumper wires

Wiring (SPI OLED + I2C BME280)
textOLED (WAVE-9092)      → ESP32
VCC                   → 3.3V
GND                   → GND
DIN (MOSI)            → GPIO 18
CLK (SCK)             → GPIO 5
CS                    → GPIO 17
DC                    → GPIO 2
RS (RST)              → GPIO 4

BME280                → ESP32
VCC                   → 3.3V
GND                   → GND
SCL                   → GPIO 33
SDA                   → GPIO 32

RGB LED
Red                   → GPIO 14 (via 220Ω)
Green                 → GPIO 27 (via 220Ω)
Blue                  → GPIO 26 (via 220Ω)
Common                → GND (common cathode)
How to Use

Connect everything as shown above
Upload the code using Arduino IDE or PlatformIO
Open Serial Monitor → see IP address (e.g., http://192.168.1.123)
Open that IP in any browser → live climate dashboard!
Red LED = too hot or too humid → open windows!
Green LED = comfortable studying conditions

Web Dashboard
Just open the IP address in your browser.
The page automatically refreshes every few seconds with fresh sensor data.

temporarily made with the help of AI 
