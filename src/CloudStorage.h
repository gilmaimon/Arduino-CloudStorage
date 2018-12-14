#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Http/Client.h"
#include "Http/EspClient.h"

template <class RequestType>
class BaseCloudStorage {
public:
  BaseCloudStorage(String baseServerUrl, String username = "", String password = ""): 
    _username(username), _password(password), _baseServerUrl(baseServerUrl) {}
  
  void setCredentials(String name, String pass) {
    this->_username = name;
    this->_password = pass;
  }

  template <class Ty>
  bool put(String key, Ty value) {    
    String jsonString = getPutRequestJson(key, value);

    RequestType request(
      _baseServerUrl + "/data/object", 
      http::Method::POST, 
      jsonString
    );
    
    request.addHeader("Content-Type", "application/json");
    http::Response response = request.execute();
    return response.statusCode == 200;
  }

  template <class Ty>
  Ty get(String key) {
    // Construct Http Request
    String jsonString = getGetRequestJson(key);

    RequestType request(
      _baseServerUrl + "/data/object", 
      http::Method::GET, 
      jsonString
    );
    
    request.addHeader("Content-Type", "application/json; charset=utf-8");
    http::Response response = request.execute();

    // Parse response and return value
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response.body);
    return getValueByKey<Ty>(root["result"], key);
  }
  
private:
  String _username, _password;
  const String _baseServerUrl;

  template <class Type>
  String getPutRequestJson(String key, Type value) {
    StaticJsonBuffer<200> jsonBuffer;
    
    JsonObject& root = jsonBuffer.createObject();
    root["username"] = _username;
    root["password"] = _password;
    root["key"] = key;
    root["value"] = value;

    String jsonString;
    root.printTo(jsonString);
    return jsonString;
  }

  String getGetRequestJson(String key) {
    StaticJsonBuffer<200> jsonBuffer;
    
    JsonObject& root = jsonBuffer.createObject();
    root["username"] = _username;
    root["password"] = _password;
    root["key"] = key;

    String jsonString;
    root.printTo(jsonString);
    return jsonString;
  }

  template <class Type>
  Type getValueByKey(JsonObject& root, String key) {
    if(key.indexOf('.') == -1) return root[key];
    String parentKey = key.substring(0, key.indexOf('.'));
    String remainder = key.substring(key.indexOf('.') + 1);

    return getValueByKey<Type>(root[parentKey], remainder);
  } 
};

typedef BaseCloudStorage<http::Esp8266Request> CloudStorage;