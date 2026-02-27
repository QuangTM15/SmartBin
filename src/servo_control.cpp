#include <Arduino.h>
#include <Servo.h>
#include "config.h"
#include "servo_control.h"

Servo servoRecycle;
Servo servoNonRecycle;

bool recycleOpen = false;
bool nonRecycleOpen = false;

unsigned long recycleTimer = 0;
unsigned long nonRecycleTimer = 0;

// debounce
unsigned long lastDebounceRecycle = 0;
unsigned long lastDebounceNonRecycle = 0;
const unsigned long debounceDelay = 50;

int lastRecycleReading = HIGH;
int lastNonRecycleReading = HIGH;

void initServos()
{
    servoRecycle.attach(SERVO_RECYCLE_PIN);
    servoNonRecycle.attach(SERVO_NONRECYCLE_PIN);

    servoRecycle.write(SERVO_CLOSE_ANGLE);
    servoNonRecycle.write(SERVO_CLOSE_ANGLE);

    pinMode(BUTTON_RECYCLE_PIN, INPUT_PULLUP);
    pinMode(BUTTON_NONRECYCLE_PIN, INPUT_PULLUP);
}

void handleButtons()
{
    int recycleReading = digitalRead(BUTTON_RECYCLE_PIN);
    int nonRecycleReading = digitalRead(BUTTON_NONRECYCLE_PIN);

    unsigned long currentTime = millis();

    // ===== Debounce recycle =====
    if (recycleReading != lastRecycleReading)
    {
        lastDebounceRecycle = currentTime;
    }

    if ((currentTime - lastDebounceRecycle) > debounceDelay)
    {
        if (recycleReading == LOW)
        {
            if (!nonRecycleOpen)
            {
                servoRecycle.write(SERVO_OPEN_ANGLE);
                recycleTimer = currentTime;
                recycleOpen = true;
                Serial.println("Recycle Open");
            }
        }
    }

    lastRecycleReading = recycleReading;

    // ===== Debounce non recycle =====
    if (nonRecycleReading != lastNonRecycleReading)
    {
        lastDebounceNonRecycle = currentTime;
    }

    if ((currentTime - lastDebounceNonRecycle) > debounceDelay)
    {
        if (nonRecycleReading == LOW)
        {
            if (!recycleOpen)
            {
                servoNonRecycle.write(SERVO_OPEN_ANGLE);
                nonRecycleTimer = currentTime;
                nonRecycleOpen = true;
                Serial.println("NonRecycle Open");
            }
        }
    }

    lastNonRecycleReading = nonRecycleReading;
}

void updateServos()
{
    unsigned long currentTime = millis();

    if (recycleOpen && (currentTime - recycleTimer >= AUTO_CLOSE_TIME))
    {
        servoRecycle.write(SERVO_CLOSE_ANGLE);
        recycleOpen = false;
        Serial.println("Recycle Closed");
    }

    if (nonRecycleOpen && (currentTime - nonRecycleTimer >= AUTO_CLOSE_TIME))
    {
        servoNonRecycle.write(SERVO_CLOSE_ANGLE);
        nonRecycleOpen = false;
        Serial.println("NonRecycle Closed");
    }
}