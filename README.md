# 🌍 IoT-Based Air Pollution Monitoring System

This project enables **real-time air quality monitoring** using ESP32 and gas sensors (MQ-135 and MQ-5). It detects various harmful gases and transmits live pollutant data to the **Blynk IoT platform** for visualization on your mobile phone.

## 🚀 Features

- 📡 Real-time monitoring of pollutants like **CO, CO₂, CH₄, NH₃, NO₂, SO₂, H₂**
- 📲 Live data visualization via the **Blynk mobile app**
- 🔔 Automatic alerts when unsafe gas thresholds are exceeded
- 🌐 MQTT cloud-based data transmission
- 💡 Indicator LEDs for visual alerts

---

## 🧰 Tech Stack

| Component         | Description                                   |
|------------------|-----------------------------------------------|
| **Microcontroller** | ESP32                                       |
| **Sensors**          | MQ-135 (air quality), MQ-5 (flammable gases) |
| **Communication**    | MQTT via Wi-Fi                             |
| **Cloud Platform**   | [Blynk IoT](https://blynk.io)              |
| **Programming**      | Arduino C++ (via Arduino IDE)              |

---

## 🔌 Hardware Setup

### 📍 Pin Connections

| Component        | ESP32 GPIO | Description                    |
|------------------|------------|--------------------------------|
| MQ-135           | A2 (GPIO 34) | Analog gas sensor input        |
| MQ-5             | A3 (GPIO 35) | Analog gas sensor input        |
| Red LED (MQ-135) | GPIO 12     | ON = MQ-135 detects danger     |
| Red LED (MQ-5)   | GPIO 26     | ON = MQ-5 detects danger       |
| Blue LED         | GPIO 13     | ON = Normal/Healthy air        |

*Power both MQ sensors with 5V and connect GND to ESP32 GND.*

---

## 📲 Blynk Setup (Cloud + Mobile)

### 1. 🔧 Blynk Cloud Setup

- Go to [https://blynk.cloud](https://blynk.cloud)
- Create a new template:
  - Template Name: `Air Pollution Monitor`
  - Hardware: `ESP32`
  - Connection Type: `WiFi`
- Note down:
  - `BLYNK_TEMPLATE_ID`
  - `BLYNK_TEMPLATE_NAME`
  - `BLYNK_AUTH_TOKEN`
- Create a new device from this template
- Add Datastreams:
  | Virtual Pin | Parameter       | Type   |
  |-------------|------------------|--------|
  | V0          | NH3              | Gauge  |
  | V1          | SO2              | Gauge  |
  | V2          | CO               | Gauge  |
  | V3          | NO2              | Gauge  |
  | V4          | CO2              | Gauge  |
  | V5          | CH4              | Gauge  |
  | V6          | H2               | Gauge  |
  | V7          | Avg MQ135 gases  | Gauge  |
  | V8          | Avg MQ5 gases    | Gauge  |

- Go to **Events** tab and create an event:
  - Event Name: `air_alert`
  - Description: `Air Quality Alert`
  - Enable **Notification**

### 2. 📱 Mobile App Setup (Android/iOS)

- Download **Blynk IoT** app from Play Store/App Store
- Log in with the same credentials
- Your created device will appear on the dashboard
- Customize widgets:
  - Add Gauges/Displays for V0 to V8
  - Add a Notification box linked to the event `air_alert`

---

## 📦 Code Configuration

Replace the following placeholders in the code:

```cpp
#define BLYNK_TEMPLATE_ID "your_template_id"
#define BLYNK_TEMPLATE_NAME "your_template_name"
#define BLYNK_AUTH_TOKEN "your_auth_token"

char ssid[] = "your_wifi_name";
char pass[] = "your_wifi_password";

```
---
## 👤 Contributors

- [Manjunatha H B](https://github.com/manjunathahb)
- [Mayura Varma Jain](https://github.com/mayuravarmajain)
