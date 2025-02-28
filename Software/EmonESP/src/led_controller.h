#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

// Enum to track LED state
enum LedState
{
    LED_OFF,
    LED_ON,
    LED_BLINKING
};

// Function prototypes
void led_setup(int pin);
void led_blink_on();
void led_blink_off();
void led_on();
void led_off();
LedState get_led_state(); // Function to retrieve the current state
void set_led_state(LedState state); // Function to set the state

#endif // LED_CONTROLLER_H
