#include "led_controller.h"

static int ledPin;        // Stores the LED pin
static LedState ledState; // Tracks the LED state

void led_setup(int pin)
{
    ledPin = pin;
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    ledState = LED_OFF; // Set initial state
}

void led_blink_on()
{
    ledState = LED_BLINKING;
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
}

void led_blink_off()
{
    ledState = LED_OFF;
    digitalWrite(ledPin, LOW);
}

void led_on()
{
    ledState = LED_ON;
    digitalWrite(ledPin, HIGH);
}

void led_off()
{
    ledState = LED_OFF;
    digitalWrite(ledPin, LOW);
}

LedState get_led_state()
{
    return ledState;
}

// Set the LED state

void set_led_state(LedState state)
{
    ledState = state;
}
