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

  // Method for storing a key/value pair
  template <class Ty>
  bool put(String key, Ty value) {
    // Build request json 
    String jsonString = buildStoreObjectRequestJson(key, value);

    // Construct http request
    RequestType request(
      _baseServerUrl + "/data/object", 
      http::Method::POST, 
      jsonString
    );
    request.addHeader("Content-Type", "application/json");
    
    // Execute request and return success status
    http::Response response = request.execute();
    return response.statusCode == 200;
  }

  // Method for retrieving key/value pair
  template <class Ty>
  Ty get(String key) {
    // Build request json 
    String jsonString = buildGetObjectRequestJson(key);
    
    // Construct http request
    RequestType request(
      _baseServerUrl + "/data/object", 
      http::Method::GET, 
      jsonString
    );
    request.addHeader("Content-Type", "application/json; charset=utf-8");
    
    //Execute request
    http::Response response = request.execute();

    // Parse response body and extract the wanted value
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response.body);
    return getValueByKey<Ty>(root["result"], key);
  }
  
private:
  String _username, _password;
  const String _baseServerUrl;

  // Utility method for constructing *Store* request json string
  template <class Type>
  String buildStoreObjectRequestJson(String key, Type value) {
    // Compose request json object
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["username"] = _username;
    root["password"] = _password;
    root["key"] = key;
    root["value"] = value;

    // Return string form of the object
    String jsonString;
    root.printTo(jsonString);
    return jsonString;
  }

  // Utility method for constructing *Get* request json string
  String buildGetObjectRequestJson(String key) {
    // Compose request json object
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["username"] = _username;
    root["password"] = _password;
    root["key"] = key;

    // Return string form of the object
    String jsonString;
    root.printTo(jsonString);
    return jsonString;
  }

  // Method for accessing nested json objects with '.' seperated keys.
  // for examples "name.first.english" for accessing name:{.., first:{ english: "MyName", .... }}
  template <class Type>
  Type getValueByKey(JsonObject& root, String key) {
    // in case no '.' is in the key, return the corresponding value
    if(key.indexOf('.') == -1) return root[key];

    // split key into parent and remainder
    // example: "user.name.fullname" => parentKey("user"), remainder("name.fullname")
    String parentKey = key.substring(0, key.indexOf('.'));
    String remainder = key.substring(key.indexOf('.') + 1);

    // traverse the parentKey object
    return getValueByKey<Type>(root[parentKey], remainder);
  } 
};

typedef BaseCloudStorage<http::Esp8266Request> CloudStorage;