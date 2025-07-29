#include <WiFi.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "values.h"  // YOUR SETTINGS

// Vars
const int defaultReReadInterval = 60000;
bool isInitialized = false;
DynamicJsonDocument latestBusInfo(1024);
unsigned int reReadInterval = defaultReReadInterval;
unsigned int lastCheckedMillis = 0;


// Init
U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE, 6, 5);

// Build URL
const String url = String(protocol) + "://" + String(server) + "/" + String(authKey) + "/" + String(stationId);
void setup() {
  Serial.begin(115200);
  oled.begin();
  oled.clearBuffer();
  oled.setFont(u8g2_font_10x20_tr);
  oled.setCursor(0, 0);
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
  if (!isInitialized || currentMillis - lastCheckedMillis > reReadInterval) {
    if (debug) {
      Serial.println("Fetching latest bus info...");
      Serial.println(currentMillis);
    }
    getLatestBusInfo();
    lastCheckedMillis = currentMillis;
  }
  oled.clearBuffer();
  if (reReadInterval == defaultReReadInterval) {
    oled.setDrawColor(1);
    oled.drawBox(0, 0, oled.getDisplayWidth(), oled.getDisplayHeight());
    oled.sendBuffer();
  }
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
  int Numver = findMinNum(latestBusInfo);
  if (Numver < (12 * 60) || Numver == defaultReReadInterval) {
    reReadInterval = defaultReReadInterval;
  } else {
    reReadInterval = (Numver - (12 * 60)) * 1000;
  }

  if (debug) {
    Serial.println(Numver);
    Serial.println(reReadInterval);
  }
}

int findMinNum(const DynamicJsonDocument& doc) {
  int minVal = INT_MAX;

  if (!doc.is<JsonArrayConst>()) return defaultReReadInterval;

  JsonArrayConst arr = doc.as<JsonArrayConst>();

  for (JsonVariantConst item : arr) {
    if (item.containsKey("EstimateTime")) {
      int estimateTimeSeconds = item["EstimateTime"].as<int>();
      minVal = estimateTimeSeconds;
    }
  }
  return (minVal == INT_MAX) ? defaultReReadInterval : minVal;
}