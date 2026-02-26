#include <Arduino.h>
#include "bluetooth_control.h"
#include "servo_control.h"

void initBluetooth() {
    Serial.begin(9600);
}

void handleBluetooth() {

    if (Serial.available()) {
        char command = Serial.read();

        if (command == 'R') {
            openRecycleBin();
        }
        else if (command == 'N') {
            openNonRecycleBin();
        }
    }
}