#include <Arduino.h>
#include "servo_control.h"

void setup()
{
    Serial.begin(9600);
    initServos();
}

void loop()
{
    handleButtons();
    updateServos();
}