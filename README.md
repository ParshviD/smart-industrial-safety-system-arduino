🚨 Smart Industrial Safety System Using Arduino

An Arduino-based, zone-wise safety monitoring system that detects fire, gas leaks, unsafe temperature, humidity, and structural vibration in real time — with instant buzzer alerts and live LCD status updates.


🧩 The Problem
Industrial sites face serious risks from fires, gas leaks, and unsafe conditions. Manual monitoring is slow, inconsistent, and often reacts only after damage is done.

💡 The Solution
An automatic, real-time monitoring system that:

Uses multiple sensors to cover different risk types
Divides the facility into independent zones, each responding to what actually matters there
Sends instant buzzer alerts and displays live readings on an LCD for quick action

🗺️ Zone Breakdown

🔥 Central Fire Zone - Fire detection	/ Flame sensor/	Buzzer on flame detection
🟥 Zone A — Combustible Materials / 	Gas & heat	MQ-2 gas sensor, LM35 temperature	/ Buzzer when gas or temp exceeds safe limits
🟦 Zone B — Raw Material or Food Storage	Environmental conditions /	DHT11 humidity, IR proximity	/ LCD displays humidity % and object presence
🟨 Zone C — Warehouse & Storage	Structural safety /	LM35 temperature, Vibration sensor / 	LCD displays temp and vibration alerts

🔧 Hardware / Core Components
Controller: Arduino Uno
Flame sensor – fire detection (active-low, digital)
MQ-2 gas sensor – combustible gas monitoring (analog)
LM35 – precise temperature measurement (x2, one per zone)
DHT11 – humidity tracking
IR sensor – proximity detection
Vibration sensor – structural monitoring
16x2 LCD (I2C) – live status display
Buzzers (x2) – Central Fire Zone buzzer + Zone A buzzer

⚙️ How It Works
The flame sensor has a 3-second startup stabilization window to avoid false triggers while readings settle.
LM35 temperature readings are averaged over 10 samples per cycle to reduce sensor noise.
The vibration sensor uses a 3-second latch: a momentary spike is held as an active alert for 3 seconds, so it isn't missed between polling cycles.
All zone readings are printed to the Serial Monitor for debugging, and key readings (humidity, temperature, IR, vibration) cycle on the LCD.

Thresholds used
MQ-2 gas threshold: 780
Zone A temperature limit: 167
Zone C temperature limit: 160
Humidity limit: 40%


📦 Libraries Required
LiquidCrystal_I2C
DHT sensor library

Install both via the Arduino IDE Library Manager before uploading.

🔌 Pin Configuration
Component	Pin
Flame sensor	D9
Central buzzer	D10
MQ-2 gas sensor	A2
LM35 (Zone A)	A0
Zone A buzzer	D11
IR sensor	D5
DHT11	D2
LM35 (Zone C)	A1
Vibration sensor	D7
LCD (I2C)	SDA/SCL

🌍 Real-World Applications
Manufacturing industries — continuous monitoring keeps work areas safe in high-risk factories
Warehouse facilities — structural integrity and environmental monitoring for large storage facilities
Food storage areas — temperature and humidity regulation to prevent spoilage in cold storage units

