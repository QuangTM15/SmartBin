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
    if (digitalRead(BUTTON_RECYCLE_PIN) == LOW && !recycleOpen)
    {
        servoRecycle.write(SERVO_OPEN_ANGLE);
        recycleTimer = millis();
        recycleOpen = true;
    }
    if (digitalRead(BUTTON_NONRECYCLE_PIN) == LOW && !nonRecycleOpen)
    {
        servoNonRecycle.write(SERVO_OPEN_ANGLE);
        nonRecycleTimer = millis();
        nonRecycleOpen = true;
    }
}

void updateServos()
{
    unsigned long currentTime = millis();

    if (recycleOpen && (currentTime - recycleTimer >= AUTO_CLOSE_TIME))
    {
        servoRecycle.write(SERVO_CLOSE_ANGLE);
        recycleOpen = false;
    }

    if (nonRecycleOpen && (currentTime - nonRecycleTimer >= AUTO_CLOSE_TIME))
    {
        servoNonRecycle.write(SERVO_CLOSE_ANGLE);
        nonRecycleOpen = false;
    }
}