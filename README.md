# Arduino-CloudStorage
Arduino Arduino library for the CloudStorage server project
Allows to easly store and retrive key/value pairs.

Example Usage:
```C++
#include <WifiConnection.h>
#include <CloudStorage.h>

CloudStorage storage("BASE_URL", "USERNAME", "PASSWORD");

void setup() {
  Serial.begin(115200);
  Serial.println("Begin");
  WifiConnection::tryConnect("WIFI_SSID", "WIFI_PASSWORD");  
  delay(3 * 1000); // Give some time to connect to wifi
}

void loop() {
  Serial.println("Checking Connection");
  if (WifiConnection::isConnected()) {
    
    // Get the current value (or 0 if dosent exist yet)
    int val = storage.get<int>("key");

    // Log current value
    Serial.print("Got value: ");
    Serial.print(val);
    Serial.println(" . Updating...");
    
    // Increment by 1 and store in the server
    storage.put<int>("key", val + 1);
    
  } else {
    Serial.println("No Connection");
  }

  delay(1000);
}
```
