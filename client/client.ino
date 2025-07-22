#include <WiFi.h>
#include <U8g2lib.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "values.h"  // YOUR SETTINGS


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, 0x3D); // This is either 0x3D or 0x3C
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    if (debug) {
      Serial.println("Connecting to WiFi...");
    }
  }
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawBox(0, 0, u8g2.getDisplayWidth(), u8g2.getDisplayHeight());
  for (int y = 0; y < u8g2.getDisplayHeight(); y++) {
    for (int x = 0; x < u8g2.getDisplayWidth(); x++) {
      if ((x + y) % 2 == 0) {
        u8g2.setDrawColor(0); 
        u8g2.drawPixel(x, y);
      }
    }
  }
  u8g2.sendBuffer();
  u8g2.setFont(u8g2_font_t0_22_tf);
}

void loop() {
    
}