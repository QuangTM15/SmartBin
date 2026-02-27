#include <Arduino.h>
#include "servo_control.h"

void setup()
{
    Serial.begin(9600);
    initServos();
    Serial.println("Smart Trash - Servo Test Start");
}

void loop()
{
    handleButtons();
    updateServos();
}