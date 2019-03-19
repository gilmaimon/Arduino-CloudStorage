#pragma once

#include <ArduinoJson.h>

class AnyValue {
public:
  AnyValue(String jsonData) : _jsonData(jsonData) {}

  template <class Ty> 
  Ty as() {
    DynamicJsonDocument root(1024);
    deserializeJson(root, _jsonData);
    
    Ty result = root["result"]["value"];
    return result;
  }
private:
  String _jsonData;
};