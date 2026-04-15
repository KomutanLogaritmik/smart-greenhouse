# smart-greenhouse
ESP32-based smart greenhouse automation with Blynk integration. Features automated climate and lighting control based on sensor data.


# Smart Greenhouse Control System (ESP32 & IoT)

This project is an automated IoT system developed to monitor and control greenhouse environmental conditions. Using an ESP32 microcontroller, sensor data is processed in real-time to manage climate and lighting, with data synchronized to both a local LCD and the Blynk IoT platform.

## 🛠️ Technical Features
* **Main Controller:** ESP32 (Integrated Wi-Fi).
* **Climate Control:** DHT11 sensor for temperature and humidity tracking. The fan is automatically activated when the temperature exceeds the set threshold.
* **Lighting Automation:** LDR-based ambient light measurement. LED lighting is activated when light levels drop below the defined limit.
* **Local Monitoring:** 20x4 I2C LCD screen for real-time status updates (Temperature, Humidity, Light, Fan Status).
* **Remote Monitoring:** Dynamic set-point adjustment and data visualization via the Blynk IoT app.

## 💡 Engineering Detail (Connection Management)
The firmware is designed with **non-blocking** logic. This ensures:
1. The system loop continues to run even if the internet connection is lost.
2. Sensor readings and automation tasks (fan/light control) remain operational offline.
3. The system automatically reconnects to the Blynk cloud once the connection is restored.

## 📂 Hardware List
* ESP32 DevKit V1
* DHT11 Temperature & Humidity Sensor
* LDR (Light Dependent Resistor)
* 20x4 I2C LCD Display
* BD139 NPN Transistor

## 🚀 Setup & Usage
1. Fill in your network credentials in the `YOUR_WIFI_SSID`, `YOUR_WIFI_PASSWORD`, and `BLYNK_AUTH_TOKEN` fields within the code.
2. Install the `Blynk`, `DHT sensor library`, and `LiquidCrystal_I2C` libraries via the Library Manager.
3. Upload the code to your ESP32 and start the system.
