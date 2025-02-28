#include <WiFi.h>
#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>

#include "config.h"
#include "led_controller.h"

#define DEVICE_NAME "ESP32_BLE_Setup"
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const int MAX_RETRIES = 10;
NimBLEServer *pServer = nullptr;
NimBLECharacteristic *pCharacteristic = nullptr;
bool deviceConnected = false;
bool isBluetoothAdvertising = false;

class ServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo)
    { // Updated signature
        deviceConnected = true;
        Serial.println("Client connected!");
        Serial.print("Client address: ");
    }

    void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason)
    { // Updated signature
        deviceConnected = false;
        isBluetoothAdvertising = false;
        Serial.println("Client disconnected!");
        Serial.print("Disconnected client address: ");
    }
};

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        std::string value = pCharacteristic->getValue();
        Serial.print("Received raw BLE data: ");

        Serial.println(value.c_str());

        if (value.length() > 0)
        {
            String receivedData = String(value.c_str());
            Serial.print("Received converted data " + receivedData);
            receivedData.trim();

            // If it's asking for MAC address, send mac address
            if (receivedData.startsWith("MAC"))
            {
                // Send WiFi MAC address to the client
                String macAddress = "MAC: " + WiFi.macAddress();
                
                Serial.println(macAddress);
                pCharacteristic->setValue(macAddress.c_str());
                pCharacteristic->notify();
            }
            else if (receivedData.startsWith("WIFI")){
                int wifiIndex = receivedData.indexOf(":");
                String ssid_and_password = receivedData.substring(wifiIndex + 1);
                int commaIndex = ssid_and_password.indexOf(',');
    
                if (commaIndex > 0)
                {
                    String ssid = ssid_and_password.substring(0, commaIndex);
                    String password = ssid_and_password.substring(commaIndex + 1);
    
                    Serial.println("Received SSID: " + ssid);
                    Serial.println("Received Password: " + password);
    
                    pCharacteristic->setValue("Connecting to WiFi...");
                    pCharacteristic->notify();
    
                    // Disconnect from any existing WiFi
                    WiFi.disconnect(true);
                    delay(1000);
    
                    WiFi.mode(WIFI_STA);
                    delay(1000);
    
                    WiFi.begin(ssid.c_str(), password.c_str());
    
                    int attempts = 0;
                    while (WiFi.status() != WL_CONNECTED && attempts < MAX_RETRIES)
                    {
                        delay(2000);
                        Serial.print(".");
                        // Print the actual WiFi status
                        Serial.print("WiFi status: ");
                        Serial.println(WiFi.status());
    
                        String statusMsg = "Attempt " + String(attempts + 1) + " of " + String(MAX_RETRIES);
                        switch (WiFi.status())
                        {
                        case WL_IDLE_STATUS:
                            statusMsg += " (IDLE)";
                            break;
                        case WL_NO_SSID_AVAIL:
                            statusMsg += " (NO SSID)";
                            break;
                        case WL_SCAN_COMPLETED:
                            statusMsg += " (SCAN DONE)";
                            break;
                        case WL_CONNECT_FAILED:
                            statusMsg += " (FAILED)";
                            break;
                        case WL_CONNECTION_LOST:
                            statusMsg += " (LOST)";
                            break;
                        case WL_DISCONNECTED:
                            statusMsg += " (DISCONNECTED)";
                            break;
                        }
                        pCharacteristic->setValue(statusMsg.c_str());
                        pCharacteristic->notify();
                        attempts++;
                    }
    
                    if (WiFi.status() == WL_CONNECTED)
                    {
                        config_save_wifi(ssid, password); // Save wifi credentials to EEPROM
                        String successMsg = "WIFI_Connected! IP: " + WiFi.localIP().toString();
                        Serial.println(successMsg);
                        pCharacteristic->setValue(successMsg.c_str());
                    }
                    else
                    {
                        String failMsg = "WIFI_ERROR Failed to connect to WiFi";
                        Serial.println(failMsg);
                        pCharacteristic->setValue(failMsg.c_str());
                    }
                    pCharacteristic->notify();
                }
            
            } else if (receivedData.startsWith("CREDS")) {
                int credsIndex = receivedData.indexOf(":");
                String credentials = receivedData.substring(credsIndex + 1);
                int commaIndex = credentials.indexOf(',');
                String modifiedMac = WiFi.macAddress();
                modifiedMac.replace(":", "_");
                mqtt_device_id = "device_" +  modifiedMac;
                mqtt_user = credentials.substring(0, commaIndex);
                mqtt_pass = credentials.substring(commaIndex + 1);

                // Inform the device that we received the credentials
                String msg = "CREDS_Success";
                pCharacteristic->setValue(msg.c_str());
                pCharacteristic->notify();
                Serial.println("Updated mqtt user and password");
                config_save_mqtt(mqtt_server, mqtt_topic, mqtt_feed_prefix, mqtt_user, mqtt_pass, true);
            }

        }
    }
};

void setup_bluetooth()
{
    isBluetoothAdvertising = true;
    Serial.begin(115200);
    Serial.println("\nStarting BLE setup...");
    // Serial.println("\nMQTT Server is ");
    // Serial.println(mqtt_server);

    // Initialize BLE device
    NimBLEDevice::init(DEVICE_NAME);

    // Set transmission power (optional, can help with connection issues)
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);

    Serial.print("ESP32 Bluetooth MAC: ");
    Serial.println(NimBLEDevice::getAddress().toString().c_str());

    // Create the BLE Server
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    // Create the BLE Service
    NimBLEService *pService = pServer->createService(SERVICE_UUID);

    // Create BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::WRITE |
            NIMBLE_PROPERTY::WRITE_NR |
            NIMBLE_PROPERTY::NOTIFY);

    pCharacteristic->setCallbacks(new CharacteristicCallbacks());

    // Start the service
    pService->start();

    // Configure advertising
    NimBLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->reset();

    // Add required advertising data
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setAppearance(0x0000);
    pAdvertising->setName(DEVICE_NAME);

    // Set advertising intervals (faster for better discovery)
    pAdvertising->setMinInterval(0x20); // 20ms interval minimum
    pAdvertising->setMaxInterval(0x40); // 40ms interval maximum

    // Start advertising
    bool advertisingStarted = pAdvertising->start();
    Serial.print("Advertising started: ");
    // digitalWrite(2, HIGH);
    Serial.println(advertisingStarted ? "Yes" : "No");
    Serial.println("BLE server ready!");
}

void loop_bluetooth()
{
    if (isBluetoothAdvertising && get_led_state() != LED_BLINKING) {
        set_led_state(LED_BLINKING);
    } else if (!isBluetoothAdvertising) {
        Serial.println("Truning off blinking!");
        set_led_state(LED_OFF);
    }
    // Handle connection state changes
    if (!deviceConnected && !isBluetoothAdvertising)
    {
        delay(500); // Give the bluetooth stack time to get ready
        Serial.println("Restarting advertising after disconnect");
        isBluetoothAdvertising = true;
        pServer->startAdvertising();
    }

    delay(200);
}