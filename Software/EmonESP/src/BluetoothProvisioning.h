#ifndef BLUETOOTH_PROVISIONING_H
#define BLUETOOTH_PROVISIONING_H

#include <Arduino.h>

// Initializes the BLE service for provisioning under the provided device name
extern void setup_bluetooth();

// Handles BLE connections and data exchange (e.g., parses Wi-Fi credentials)
extern void loop_bluetooth();

// Connects to the Wi-Fi network using provided SSID and password
extern void connectToWiFi(const String& ssid, const String& password);

#endif
