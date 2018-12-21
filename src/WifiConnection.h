#pragma once

#include <WiFi.h>
#include <WiFiMulti.h>

namespace WifiConnection {  
  WiFiMulti wifiMulti;
  
  // Sets wifi mode to STA and tries to connect to a network
  void tryConnect(String ssid, String pass) {
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(ssid.c_str(), pass.c_str());
  }
  
  // Method for checking if currently connected to wifi
  boolean isConnected() {
    return wifiMulti.run() == WL_CONNECTED;
  }
};
