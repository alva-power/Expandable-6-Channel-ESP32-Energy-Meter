/*
   -------------------------------------------------------------------
   EmonESP Serial to Emoncms gateway
   -------------------------------------------------------------------
   Adaptation of Chris Howells OpenEVSE ESP Wifi
   by Trystan Lea, Glyn Hudson, OpenEnergyMonitor

   Modified to use with the CircuitSetup.us Split Phase Energy Meter by jdeglavina

   All adaptation GNU General Public License as below.

   -------------------------------------------------------------------

   This file is part of OpenEnergyMonitor.org project.
   EmonESP is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.
   EmonESP is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with EmonESP; see the file COPYING.  If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

// See energy meter specific configuration in energy_meter.h
#define ENABLE_ENERGY_METER

// If USE_SERIAL_INPUT is defined, EmonESP will check the serial port for
// input. Only enable this if there is input expected on the serial port;
// otherwise it seems to read garbage data.
// #define USE_SERIAL_INPUT

#include <esp_task_wdt.h>
#include "emonesp.h"
#include "config.h"
#include "esp_wifi.h"
#include "web_server.h"
#include "ota.h"
#include "input.h"
#include "emoncms.h"
#include "mqtt.h"
#include "BluetoothProvisioning.h"
#include "led_controller.h"
#ifdef ENABLE_ENERGY_METER
#include "energy_meter.h"
#endif
#define LED_PIN 2 // Change according to your board
static char input[MAX_DATA_LEN];
const int MAX_RETRIES = 10;
// -------------------------------------------------------------------
// SETUP
// -------------------------------------------------------------------
void setup()
{

#ifdef ENABLE_WDT
  enableLoopWDT();
#endif

  Serial.begin(115200);
#ifdef DEBUG_SERIAL1
  Serial1.begin(115200);
#endif
  delay(200);

  led_setup(LED_PIN);
  delay(200);

  // Read saved settings from the config
  config_load_settings();

  // Set MQTT topic
  String macAddressMqtt = WiFi.macAddress();

  // Replace colons with underscores
  macAddressMqtt.replace(":", "_");

  String modifiedMac = WiFi.macAddress();
  modifiedMac.replace(":", "_");
  mqtt_device_id = "device_" + modifiedMac;

  // Construct the MQTT topic
  mqtt_topic = "devices/device_" + macAddressMqtt + "/messages/events/";

  Serial.print("MQTT TOPIC IS BEING SET IN THE SETUP NOW: ");
  Serial.println(mqtt_topic);

  // Check if we have valid wifi username and password.
  if (esid.length() == 0 || epass.length() == 0)
  // if (true)
  {
    DBUGS.println("No WiFi credentials saved. Starting provisioning.");

    // Start provisioning
    setup_bluetooth();
  }
  else
  {
    DBUGS.println("WiFi credentials saved. Starting normal operation." + esid + " " + epass);
    Serial.print("SSID: ");
    Serial.println(esid);
    Serial.print("Password: ");
    Serial.println(epass);

    WiFi.disconnect(true);
    delay(1000);
    // Try connecting wifi
    WiFi.mode(WIFI_STA);
    delay(1000);

    WiFi.begin(esid.c_str(), epass.c_str());

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
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      config_save_wifi(esid, epass); // Save wifi credentials to EEPROM
      String successMsg = "WIFI_Connected! IP: " + WiFi.localIP().toString();
    }
    else
    {
      String failMsg = "WIFI_ERROR Failed to connect to WiFi";
      Serial.println(failMsg);
      setup_bluetooth();
    }
  }
  // If not start bluetooth.
  // If yes, start wifi
  // if wifi connection failed, change to bluetooth

  delay(200);

  // Initialise the WiFi
  // wifi_setup();
  delay(200);

  delay(200);

  // Bring up the web server
  // web_server_setup();
  delay(200);

#ifdef ENABLE_WDT
  DBUGS.println("Watchdog timer is enabled.");
  feedLoopWDT();
#endif

#ifdef ENABLE_WEB_OTA
  // Start the OTA update systems
  ota_setup();
#endif

#ifdef ENABLE_ENERGY_METER
  energy_meter_setup();
#endif
  digitalWrite(2, LOW);

  DBUGLN("Server started");

} // end setup

// -------------------------------------------------------------------
// LOOP
// -------------------------------------------------------------------
void loop()
{
  if (get_led_state() == LED_BLINKING)
  {
    led_blink_on();
  }
  else
  {
    led_blink_off();
  }
  // digitalWrite(2, LOW);
#ifdef ENABLE_WDT
  // feedLoopWDT();
#endif
  // web_server_loop();
  // wifi_loop();
  loop_bluetooth();

#ifdef ENABLE_WEB_OTA
  // ota_loop();
#endif

#ifdef ENABLE_ENERGY_METER
  energy_meter_loop();
#endif

  boolean gotInput = input_get(input);
  if (gotInput)
  {
    DBUGS.println(".");
  }

  if (wifi_client_connected())
  {
    if (emoncms_apikey != 0 && gotInput)
    {
      // DBUGS.println(input);
      if (emoncms_server != 0)
      {
        emoncms_publish(input);
      }
    }
    if (mqtt_server != 0)
    {
      mqtt_loop();
      if (gotInput)
      {
        // print mqtt_server and mqtt_topic, mqtt_feed_prefix, mqtt_user, mqtt_pass
        Serial.print("MQTT Server: ");
        Serial.println(mqtt_server);
        Serial.print("MQTT Topic: ");
        Serial.println(mqtt_topic);
        Serial.print("MQTT user: ");
        Serial.println(mqtt_user);
        Serial.print("MQTT password: ");
        Serial.println(mqtt_pass);
        mqtt_publish(input);
      }
    }
  }

} // end loop
