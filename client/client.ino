#include <WiFi.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "values.h"  // YOUR SETTINGS

// Vars
const int reReadInterval = 60000;
bool isInitialized = false;
unsigned long lastGetLatestBusInfo = 0;
DynamicJsonDocument latestBusInfo(1024);

int currentBusIndex = 0;
unsigned long lastBusSwitch = 0;
const unsigned long busSwitchInterval = 1000;

// Init
U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE, 6, 5);

// Build URL
const String url = String(protocol) + "://" + String(server) + "/" + String(authKey) + "/" + String(stationId);
void setup() {
  Serial.begin(115200);
  oled.begin();
  oled.clearBuffer();
  oled.setFont(u8g2_font_10x20_tr);
  oled.setCursor(0, 20);
  oled.print("......");
  oled.sendBuffer();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    if (debug) {
      Serial.println("Connecting to WiFi...");
    }
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (!isInitialized || currentMillis - lastGetLatestBusInfo >= reReadInterval) {
    if (debug) {
      Serial.println("Fetching latest bus info...");
    }
    getLatestBusInfo();
    lastGetLatestBusInfo = currentMillis;
    currentBusIndex = 0;
  }
  if (latestBusInfo.size() > 0 && currentMillis - lastBusSwitch >= busSwitchInterval) {
    currentBusIndex = (currentBusIndex + 1) % latestBusInfo.size();
    lastBusSwitch = currentMillis;
  }
  oled.clearBuffer();
  oled.setFont(u8g2_font_10x20_tr);
  oled.setCursor(0, 20);

  if (latestBusInfo.size() > 0) {
    oled.print(latestBusInfo[currentBusIndex]["RouteName"]["En"].as<String>());
  } else {
    oled.print("No Data");
  }
  oled.setCursor(10, 40);
  oled.setFont(u8g2_font_callite24_tr);
  oled.print(latestBusInfo[currentBusIndex]["EstimateTime"].as<String>());
  oled.sendBuffer();
  delay(1000);
}

void getLatestBusInfo() {
  HTTPClient http;
  http.begin(url.c_str());
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String payload = http.getString();
    latestBusInfo = DynamicJsonDocument(1024);
    DeserializationError error = deserializeJson(latestBusInfo, payload);
    if (debug) {
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      };
      Serial.println(payload);
    }
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  isInitialized = true;
}