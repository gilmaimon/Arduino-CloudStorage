#pragma once

#include <WiFi.h>

namespace WifiConnection {    
  // Sets wifi mode to STA and tries to connect to a network
  void tryConnect(String ssid, String pass) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
  }
  
  // Method for checking if currently connected to wifi
  boolean isConnected() {
    return WiFi.status() == WL_CONNECTED;
  }
};
