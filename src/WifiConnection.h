#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

namespace WifiConnection {  
  ESP8266WiFiMulti WiFiMulti;
  
  // Sets wifi mode to STA and tries to connect to a network
  void tryConnect(String ssid, String pass) {
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid.c_str(), pass.c_str());
  }
  
  // Method for checking if currently connected to wifi
  boolean isConnected() {
    return WiFiMulti.run() == WL_CONNECTED;
  }
};
