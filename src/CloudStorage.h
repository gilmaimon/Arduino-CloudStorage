#pragma once

#include <ArduinoJson.h>
#include "Http/Client.h"
#include "Http/EspClient.h"
#include "Utils/ResultWrapper.h"

enum PopFrom {
  PopFrom_Start,
  PopFrom_End
};
  
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
    if(response.statusCode != 200) return false;

    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response.body);

    return !root["error"]; // return isOk
  }

  // Method for retrieving key/value pair
  template <class Ty>
  cloud_storage_utils::ResultWrapper<Ty> get(String key) {
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
    if(response.statusCode != 200) return false;

    // Parse response body and extract the wanted value
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response.body);
    return cloud_storage_utils::ResultWrapper<Ty>(
      !root["error"],
      getValueByKey<Ty>(root["result"], key)
    );
  }

  //Method for pushing new values to arrays
  template <class Ty>
  bool add(String collectionKey, Ty object) {
    // Build request json 
    String jsonString = buildAddObjectRequestJson(collectionKey, object);

    // Construct http request
    RequestType request(
      _baseServerUrl + "/data/collection", 
      http::Method::POST, 
      jsonString
    );
    request.addHeader("Content-Type", "application/json; charset=utf-8");

    // Execute request and return success status
    http::Response response = request.execute();
    if(response.statusCode != 200) return false;
    
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response.body);

    return !root["error"]; // return isOk
  }

  // Method for popping values from arrays in the server
  template <class Ty>
  cloud_storage_utils::PopResultWrapper<Ty> pop(String collectionKey, PopFrom popFrom) {
    // Build request json 
    String jsonString = buildPopRequestJson(collectionKey, popFrom);

    // Construct http request
    RequestType request(
      _baseServerUrl + "/data/collection/pop", 
      http::Method::GET, 
      jsonString
    );
    request.addHeader("Content-Type", "application/json; charset=utf-8");

    // Execute request and return success status
    http::Response response = request.execute();
    if(response.statusCode != 200) return false;
    
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response.body);

    if(root["error"]) {
      return false;
    }

    return cloud_storage_utils::PopResultWrapper<Ty>(
      true,
      root["result"]["value"],
      !root["result"]["empty"]
    );
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

  // Utility method for constructing *Add* request json string
  template <class Ty>
  String buildAddObjectRequestJson(String collectionKey, Ty object) {
    // Compose request json object
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["username"] = _username;
    root["password"] = _password;
    root["collection_key"] = collectionKey;
    root["value"] = object;

    // Return string form of the object
    String jsonString;
    root.printTo(jsonString);
    return jsonString;
  }

  // Utility method for constructing *Pop* request json string
  String buildPopRequestJson(String collectionKey, PopFrom popFrom) {
    // Compose request json object
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["username"] = _username;
    root["password"] = _password;
    root["collection_key"] = collectionKey;
    root["position"] = (popFrom == PopFrom_Start? "first": "last");

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