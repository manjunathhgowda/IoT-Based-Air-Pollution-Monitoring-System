#define BLYNK_TEMPLATE_ID "TMPL3iZjOwTo8"
#define BLYNK_TEMPLATE_NAME "Air Pollution Monitoring System"
#define BLYNK_AUTH_TOKEN "sRsAr468g0LpQm1Tfo7GYZce4xNGf1e8"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "mayuravarma";
char pass[] = "mayuravarma";

// MQ Sensor Pins
#define MQ135_PIN     34
#define MQ5_PIN       35
#define RED_LED_MQ135 12
#define RED_LED_MQ5   26
#define BLUE_LED      13

// Thresholds
float CO_THRESHOLD   = 500.0; //400
float NH3_THRESHOLD  = 100.0; 
float CO2_THRESHOLD  = 800.0; //500-600
float SO2_THRESHOLD  = 300.0; //200
float NO2_THRESHOLD  = 600.0; //400
float CH4_THRESHOLD  = 20.0;
float H2_THRESHOLD   = 40.0;

float RL_MQ5 = 10.0, RL_MQ135 = 10.0;
float Ro_MQ5 = 5.0, Ro_MQ135 = 10.0;

bool alertSent = false;

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED_MQ135, OUTPUT);
  pinMode(RED_LED_MQ5, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, HIGH);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  int mq135ADC = analogRead(MQ135_PIN);
  int mq5ADC = analogRead(MQ5_PIN);

  float mq135V = mq135ADC * (3.3 / 4095.0);
  float mq5V = mq5ADC * (3.3 / 4095.0);

  float Rs_MQ135 = (3.3 - mq135V) * RL_MQ135 / mq135V;
  float Rs_MQ5 = (3.3 - mq5V) * RL_MQ5 / mq5V;

  float ratio135 = Rs_MQ135 / Ro_MQ135;
  float ratio5 = Rs_MQ5 / Ro_MQ5;

  // MQ-135 Gases
  float nh3_ppm  = 10 * pow(ratio135, -1.5);
  float so2_ppm  = 100 * pow(ratio135, -1.3);
  float co_ppm   = 116.6 * pow(ratio135, -2.76);
  float no2_ppm  = 200 * pow(ratio135, -1.8);
  float co2_ppm  = 400 * pow(ratio135, -1.1);

  // MQ-5 Gases
  float ch4_ppm = 500 * pow(ratio5, -2.5);
  float h2_ppm  = 1300 * pow(ratio5, -1.8);

  // Print to Serial
  Serial.println("\n=== AIR SENSOR READINGS ===");
  Serial.printf("NH3: %.2f ppm | SO2: %.2f ppm | CO: %.2f ppm\n", nh3_ppm, so2_ppm, co_ppm);
  Serial.printf("NO2: %.2f ppm | CO2: %.2f ppm\n", no2_ppm, co2_ppm);
  Serial.printf("CH4: %.2f ppm | H2: %.2f ppm\n", ch4_ppm, h2_ppm);
  Serial.println("===========================\n");

  // Send to Blynk Gauges
  Blynk.virtualWrite(V0, nh3_ppm);
  Blynk.virtualWrite(V1, so2_ppm);
  Blynk.virtualWrite(V2, co_ppm);
  Blynk.virtualWrite(V3, no2_ppm);
  Blynk.virtualWrite(V4, co2_ppm);
  Blynk.virtualWrite(V5, ch4_ppm);
  Blynk.virtualWrite(V6, h2_ppm);

  float avg_mq135 = (nh3_ppm + so2_ppm + co_ppm + no2_ppm + co2_ppm) / 5.0;
  float avg_mq5 = (ch4_ppm + h2_ppm) / 2.0;

  Blynk.virtualWrite(V7, avg_mq135);
  Blynk.virtualWrite(V8, avg_mq5);

  // Threshold Checks
  bool mq135_danger = (co_ppm > CO_THRESHOLD) || (co2_ppm > CO2_THRESHOLD) ||
                      (nh3_ppm > NH3_THRESHOLD) || (so2_ppm > SO2_THRESHOLD) ||
                      (no2_ppm > NO2_THRESHOLD);

  bool mq5_danger = (ch4_ppm > CH4_THRESHOLD) || (h2_ppm > H2_THRESHOLD);

  digitalWrite(RED_LED_MQ135, mq135_danger ? HIGH : LOW);
  digitalWrite(RED_LED_MQ5, mq5_danger ? HIGH : LOW);
  digitalWrite(BLUE_LED, (!mq135_danger && !mq5_danger) ? HIGH : LOW);

  // Alert with plain text formatting
  if ((mq135_danger || mq5_danger) && !alertSent) {
    String body = "⚠ AIR QUALITY ALERT ⚠\n\n";
    body += "Gases above safe threshold:\n";

    if (nh3_ppm > NH3_THRESHOLD) body += "- NH3 (Ammonia): " + String(nh3_ppm, 2) + " ppm\n";
    if (so2_ppm > SO2_THRESHOLD) body += "- SO2 (Sulfur Dioxide): " + String(so2_ppm, 2) + " ppm\n";
    if (co_ppm > CO_THRESHOLD) body += "- CO (Carbon Monoxide): " + String(co_ppm, 2) + " ppm\n";
    if (no2_ppm > NO2_THRESHOLD) body += "- NO2 (Nitrogen Dioxide): " + String(no2_ppm, 2) + " ppm\n";
    if (co2_ppm > CO2_THRESHOLD) body += "- CO2 (Carbon Dioxide): " + String(co2_ppm, 2) + " ppm\n";
    if (ch4_ppm > CH4_THRESHOLD) body += "- CH4 (Methane): " + String(ch4_ppm, 2) + " ppm\n";
    if (h2_ppm > H2_THRESHOLD) body += "- H2 (Hydrogen): " + String(h2_ppm, 2) + " ppm\n";

    body += "\n--- Full Gas Report ---\n";
    body += "NH3: " + String(nh3_ppm, 2) + " ppm\n";
    body += "SO2: " + String(so2_ppm, 2) + " ppm\n";
    body += "CO: " + String(co_ppm, 2) + " ppm\n";
    body += "NO2: " + String(no2_ppm, 2) + " ppm\n";
    body += "CO2: " + String(co2_ppm, 2) + " ppm\n";
    body += "CH4: " + String(ch4_ppm, 2) + " ppm\n";
    body += "H2: " + String(h2_ppm, 2) + " ppm\n";

    Blynk.logEvent("pollution_alert", body);
    alertSent = true;
  }

  if (!mq135_danger && !mq5_danger) alertSent = false;

  delay(3000); // 3 seconds delay between readings
}


