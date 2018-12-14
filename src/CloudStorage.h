#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Http/Client.h"
#include "Http/EspClient.h"

const String BaseServerUrl = "http://192.168.43.73:8080";

template <class RequestType>
class BaseCloudStorage {
public:
  BaseCloudStorage(String username = "", String password = ""): _username(username), _password(password) {}
  
  void setCredentials(String name, String pass) {
    this->_username = name;
    this->_password = pass;
  }

  template <class Ty>
  bool put(String key, Ty value) {    
    String jsonString = getPutRequestJson(key, value);

    RequestType request(
      BaseServerUrl + "/data/object", 
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
      BaseServerUrl + "/data/object", 
      http::Method::GET, 
      jsonString
    );
    
    request.addHeader("Content-Type", "application/json");
    http::Response response = request.execute();

    // Parse response and return value
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response.body);
    return root["result"][key];
  }
  
private:
  String _username;
  String _password;

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
};

typedef BaseCloudStorage<http::Esp8266Request> CloudStorage;