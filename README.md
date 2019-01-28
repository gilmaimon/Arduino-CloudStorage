[![arduino-library-badge](https://www.ardu-badge.com/badge/ArduinoCloudStorage.svg)](https://www.ardu-badge.com/ArduinoCloudStorage)  [![Build Status](https://travis-ci.org/gilmaimon/Arduino-CloudStorage.svg?branch=master)](https://travis-ci.org/gilmaimon/Arduino-CloudStorage)
# Arduino-CloudStorage
Arduino/ESP8266 library that allows you to easly store and retreive data from a remote (cloud) storage. CloudStorage server can be found [here](https://github.com/gilmaimon/CloudStorage-Server).
*Open an issue or contact me if you need any help!*

Quick Jump:
* [How To Install](#How-To-Install)
* [Features & Syntax](#features--syntax)
* [Requirements](#requirements)
* [Public Experiment Server](#Public-Experiment-Server)
* [Known Issues](#known-issues)
* [Examples](#examples)

# How To Install
1. Just go to your local folder for Arduino projects (usually `User/Documents/Arduino/`).
2. Step into the folder called `libraries`
3. Clone the repository in the `libraries` folder and restart the Arduino IDE.
4. Should be good to go! Try to load the example sketches from the menu, "File" -> Examples" -> "ArduinoCloudStorage"

For more information on installing libraries, see: http://www.arduino.cc/en/Guide/Libraries

# Features & Syntax
1. Store key/value pairs to the server
```C++
storage.put<int>("age", 20);
storage.put<float>("temperature", 25.8);
storage.put<bool>("switch_state", false);
```
2. Fetch key/value pairs from the server
```C++
int age = storage.get<int>("age");
float temperature = storage.get<float>("temperature");
bool isSwitchOn = storage.get<bool>("switch_state");
```
3. Add values to an array in the server
```C++
storage.add<float>("sensor_values", getCurrentSensorValue());
storage.add<String>("startup_dates", "17.12.2018");
```
4. Pop values from an array in the server
```C++
String color = storage.pop<String>("colors_to_show", PopFrom_End);
float temp = storage.pop<float>("tempertures", PopFrom_Start);
```
```C++
// More advanced example of pop. In the previous example the returned value is 
// implicitly casted into the required return type (String, float etc..)
while(true) {
  auto popResult = storage.pop<String>("lines_to_print", PopFrom_Start);
  if(popResult.isOK == false) break;
  proccessLine(popResult.value);
  if(popResult.hasNext == false) break;
}
```
5. Get an aggregate of a collection
```
int average = storage.avg("samples");
int min = storage.min<int>("samples");
int max = storage.max<int>("samples");
```


# Requirements
* This library uses the `ArduinoJson` library as a depedency - version 5.13.4 (IMPORTANT).
* You must have an instance of [CloudStorage Server](https://github.com/gilmaimon/CloudStorage-Server) running.
* You must have valid `username` and `password` (on your CloudStorage Server)

# Public Experiment Server
*As of 18.12.2018, you could use my public instance as server (As `BASE_URL`): http://cloudstorage.gilmaimon.xyz:5050*. 
*You can [Click Here](http://cloudstorage.gilmaimon.xyz:5050/user/register) to register a new user on that server.*
11.01.2019 Update: Server moved to port 5050.

**NOTE**:
* This server will not stay alive forever. 
* IP and/or Port might change, take that into considiration.
* Please, don't abuse bandwith/storage (IP will be blocked and/or server will be taken down)

# Known Issues
* Bad error reporting and handling
* No explanation on setting up a server.

# Examples

### Increment Value in Server
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

### Log Sensor Values
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
*Open an issue or contact me if you need any help!*
