# Bus API & OLED screen for ESP32 and SSD1306
![](https://hackatime-badge.hackclub.com/U087ATD163V/bus-api)

This is a simple client & api for the Taiwan TDX system, which is where you can get the government public transport data. This is made using ESP32C3 and a SSD1306 OLED screen.

This is submitted to SoM: https://summer.hackclub.com/projects/7617/

## How can you try it?
First, register the TDX api from here: https://tdx.transportdata.tw/

Note that you **MUST** be a Taiwanese citizen to register the TDX api!

Second, get your ESP32 C3 and a SSD1306 OLED screen.

Third, download the entire repo, and open client.ino in the Arduino IDE.

Forth, install the required libraries and the offical ESP32 source.
1. Put `https://dl.espressif.com/dl/package_esp32_index.json` into the settings text box.
2. Go to the board manager at the sidebar and search for ESP32, and download the offical ESP32 package (**NOT THE ARDUNIO PACKAGE**)
3. Go the library manager and download a few packages, this includes: U8G2 & ArduinoJson
4. Flash it!
