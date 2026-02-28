#include <Arduino.h>
#include "bluetooth_control.h"
#include "servo_control.h"

void setup()
{
    Serial.begin(9600);
    initServos();
    initBluetooth();
}

void loop()
{
    handleButtons();
    updateServos();
    handleBluetooth();
}