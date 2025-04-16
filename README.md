# Smart Agri System 🌱🚿

A simple Smart Agriculture System that uses **Machine Learning (Random Forest)** for intelligent irrigation decisions. This project integrates both hardware components and a machine learning model to automate and optimize irrigation based on environmental conditions.

---

## 🌟 Features

- 📊 **Random Forest ML Model** for irrigation prediction.
- 🌧️ **Rain Sensor** to prevent irrigation during rain.
- 🌱 **Soil Moisture Sensor** to assess soil water content.
- 🌡️ **Temperature & Humidity Sensor (DHT11)** for environmental monitoring.
- ⏰ **Real-Time Clock (RTC)** for time-based irrigation rules (e.g., early morning, noon).
- 💧 **Water Level Sensor** to check tank/reservoir levels.
- 📟 **LCD Display** to show system messages and decisions.
- 🔄 **Serial Communication using COMPIM** between circuit and ML model.
- 💡 **Arduino UNO** as the main controller for sensor integration.

---

## 🧠 How It Works

1. **Sensor data** (rain, soil moisture, temperature, humidity, time, and water level) is collected via Arduino.
2. Data is sent to a **Python-based Random Forest model** via **COMPIM** (serial communication).
3. The model predicts whether irrigation should occur based on:
   - Soil dryness
   - Rain presence
   - Time of day (irrigation only happens early morning or at noon when there’s no sun)
4. If conditions are met, irrigation is triggered, and messages are displayed on the **LCD screen**.

---

## 🛠️ Technologies & Tools

- **Arduino UNO**
- **Sensors**: Soil Moisture, Rain, DHT11 (Temp/Humidity), RTC, Water Level
- **Python**: For the ML model and serial communication
- **COMPIM**: Virtual serial communication interface
- **Proteus**: For circuit simulation
- **Random Forest Classifier**: Machine Learning algorithm used

---


