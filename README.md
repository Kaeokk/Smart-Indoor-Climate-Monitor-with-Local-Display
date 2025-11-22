
Smart Indoor Climate Monitor
Library Study Room Comfort System

Project Overview
This IoT device continuously monitors temperature, humidity, and air pressure using a BME280 sensor to help detect stuffy or uncomfortable library study rooms.

Real-time feedback is provided through:

Clear 128×64 OLED display
RGB LED visual alerts (Green = comfortable, Red = too hot/humid)
Simple web dashboard accessible from any phone or laptop on the same Wi-Fi
Perfect for students and facilities staff!

Features Implemented
Core Requirements (All Completed)

BME280 sensor integration via I2C
Live local display on OLED
Built-in web server (no internet needed)
RGB LED visual warnings
Enhanced Features (Completed)

Customizable alert thresholds (28°C / 60% RH default)
Clean web interface with auto-refresh
Pressure displayed in hPa (user-friendly)
Hardware Required
ESP32 Development Board
BME280 Temperature/Humidity/Pressure Sensor
0.96" SSD1306 OLED Display (128×64, SPI)
Common-Cathode RGB LED + 3× 220Ω resistors
Breadboard and jumper wires
Micro-USB cable
Wiring Diagram (All Common Ground)
text
OLED Display (WAVE-9092 SPI)       → ESP32
──────────────────────────────────────────────
VCC                                → 3.3V
GND                                → GND
DIN  (MOSI)                        → GPIO 18
CLK  (SCK)                         → GPIO 5
CS                                 → GPIO 17
DC                                 → GPIO 2
RST (RS)                           → GPIO 4

BME280 Sensor (I2C)                → ESP32
──────────────────────────────────────────────
VCC                                → 3.3V
GND                                → GND
SCL                                → GPIO 33
SDA                                → GPIO 32

RGB LED (Common Cathode)           → ESP32
──────────────────────────────────────────────
Red leg                            → GPIO 14 (via 220Ω resistor)
Green leg                          → GPIO 27 (via 220Ω resistor)
Blue leg                           → GPIO 26 (via 220Ω resistor)
Common Cathode (longest leg)       → GND
How to Use
Wire everything exactly as shown above
Upload the code using Arduino IDE or PlatformIO
Open Serial Monitor (115200 baud) → note the IP address
Example: IP Address: http://192.168.1.105
Open that IP in any browser (phone, laptop, tablet)
Live climate dashboard appears — updates automatically!
LED Status
Green → Comfortable (good to study)
Red → Too hot or too humid → open a window!

Web Dashboard
Just type the IP address into any web browser.
The page refreshes every 5 seconds with fresh sensor readings.

temporarily for fun made with the help of AI 
