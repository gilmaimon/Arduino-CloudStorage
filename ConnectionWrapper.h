#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

namespace WifiConnection {  
  ESP8266WiFiMulti WiFiMulti;
  
  void tryConnect(String ssid, String pass) {
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid.c_str(), pass.c_str());
  }
  
  boolean isConnected() {
    return WiFiMulti.run() == WL_CONNECTED;
  }
};
