#pragma once

#include <ArduinoJson.h>

class AnyValue {
public:
  AnyValue(String jsonData) : _jsonData(jsonData) {}

  template <class Ty> 
  Ty as() {
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(_jsonData);
    
    Ty result = root["result"]["value"];
    return result;
  }
private:
  String _jsonData;
};