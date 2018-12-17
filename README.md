# Arduino-CloudStorage
Arduino/ESP8266 library that allows:
1. Store key/value pairs to the server
2. Fetch key/value pairs from the server
3. Add values to an array in the server.

# Requirements
* This library uses the `ArduinoJson` library as a depedency.
* You must have an instance of [CloudStorage Server](https://github.com/gilmaimon/CloudStorage-Server) running.
* You must have valid `username` and `password` (on your CloudStorage Server)
# Examples

#### Increment Value in Server
This example sketch:
1. Connects to a WiFi network
2. Initializes a value in the server
3. Increment the value in `loop()`

```C++
CloudStorage storage("BASE_URL", "USERNAME", "PASSWORD");

void setup() {
  Serial.begin(115200);
  
  // Try to connect to a wifi network
  WifiConnection::tryConnect("WIFI_SSID", "WIFI_PASSWORD");  

  // Give the ESP some time to connect
  delay(3 * 1000);
  
  // initialize value to 0
  if (WifiConnection::isConnected()) {
    storage.put<int>("count", 0);
  }

}

void loop() {
  Serial.println("Checking Connection");
  if (WifiConnection::isConnected()) {
    
    // Get the current value
    int val = storage.get<int>("count");

    // Log
    Serial.print("Got value: ");
    Serial.print(val);
    Serial.println(" . Updating...");
    
    // Increment by 1 and store in the server
    storage.put<int>("count", val + 1);
    
  } else {
    Serial.println("No Connection");
  }

  delay(1000);
}
```

#### Log Sensor Values
This example sketch:
1. Connects to a WiFi network
2. Then, in `loop()`: reads and logs sensor value to the server

```C++
CloudStorage storage("BASE_URL", "USERNAME", "PASSWORD");

void setup() {
  Serial.begin(115200);
  
  // Try to connect to a wifi network
  WifiConnection::tryConnect("WIFI_SSID", "WIFI_PASSWORD");  

  // Give the ESP some time to connect
  delay(3 * 1000);
}

void loop() {
  Serial.println("Checking Connection");
  if (WifiConnection::isConnected()) {
    
    // get some sensor value to log
    float value = getSomeSensorValue();

    // add that value to array in the server
    storage.add("sensor_values", value);
    
  } else {
    Serial.println("No Connection");
  }

  delay(5 * 1000);
}
```
