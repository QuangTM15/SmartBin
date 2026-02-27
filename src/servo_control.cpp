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

// Debounce variables
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

// ========================
// OPEN FUNCTIONS
// ========================

void openRecycle()
{
    // Chỉ mở nếu ngăn kia không mở
    if (!nonRecycleOpen)
    {
        servoRecycle.write(SERVO_OPEN_ANGLE);
        recycleTimer = millis();
        recycleOpen = true;
        Serial.println("Recycle Open");
    }
}

void openNonRecycle()
{
    if (!recycleOpen)
    {
        servoNonRecycle.write(SERVO_OPEN_ANGLE);
        nonRecycleTimer = millis();
        nonRecycleOpen = true;
        Serial.println("NonRecycle Open");
    }
}

// ========================
// HANDLE BUTTONS (Debounce)
// ========================

void handleButtons()
{
    unsigned long currentTime = millis();

    int recycleReading = digitalRead(BUTTON_RECYCLE_PIN);
    int nonRecycleReading = digitalRead(BUTTON_NONRECYCLE_PIN);

    // ---- Debounce Recycle ----
    if (recycleReading != lastRecycleReading)
    {
        lastDebounceRecycle = currentTime;
    }

    if ((currentTime - lastDebounceRecycle) > debounceDelay)
    {
        if (recycleReading == LOW)
        {
            openRecycle();
        }
    }

    lastRecycleReading = recycleReading;

    // ---- Debounce NonRecycle ----
    if (nonRecycleReading != lastNonRecycleReading)
    {
        lastDebounceNonRecycle = currentTime;
    }

    if ((currentTime - lastDebounceNonRecycle) > debounceDelay)
    {
        if (nonRecycleReading == LOW)
        {
            openNonRecycle();
        }
    }

    lastNonRecycleReading = nonRecycleReading;
}

// ========================
// AUTO CLOSE SYSTEM
// ========================

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