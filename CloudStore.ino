#include <Arduino.h>

#include "Http/EspClient.h"
#include "ConnectionWrapper.h"

#include <ArduinoJson.h>

const String baseServerUrl = "http://192.168.43.73:8080";

class CloudStorage {
public:
  CloudStorage(String username = "", String password = ""): _username(username), _password(password) {}
  
  void setCredentials(String name, String pass) {
    this->_username = name;
    this->_password = pass;
  }

  template <class Ty>
  bool put(String key, Ty value) {    
    String jsonString = getPutRequestJson(key, value);

    http::Esp8266Request request(
      baseServerUrl + "/data/object", 
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

    http::Esp8266Request request(
      baseServerUrl + "/data/object", 
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

CloudStorage storage("gil", "123456");

void setup() {
  Serial.begin(115200);
  Serial.println("Begin");
  WifiConnection::tryConnect("The Promised Lan", "251998gil");  
  delay(3 * 1000); // Give some time to connect to wifi
}

void loop() {
  Serial.println("Checking Connection");
  if (WifiConnection::isConnected()) {
    
    int val = storage.get<int>("age");
    
    Serial.print("Got value: ");
    Serial.print(val);
    Serial.println(" . Updating...");
    
    storage.put<int>("age", val + 1);
    
  } else {
    Serial.println("No Connection");
  }

  delay(10000);
}

