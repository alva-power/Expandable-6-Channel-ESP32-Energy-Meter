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

  Serial.begin(115200);

  led_setup(LED_PIN);
  delay(200);

  // Read saved settings from the config
  config_load_settings();

  energy_meter_setup();

  digitalWrite(2, LOW);

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
  energy_meter_loop();

} // end loop
