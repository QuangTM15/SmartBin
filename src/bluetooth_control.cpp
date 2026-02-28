#include <Arduino.h>
#include <SoftwareSerial.h>
#include "bluetooth_control.h"
#include "servo_control.h"
#include "config.h"

SoftwareSerial bluetooth(BT_RX_PIN, BT_TX_PIN);

void initBluetooth()
{
    bluetooth.begin(9600);
}

void handleBluetooth()
{
    if (bluetooth.available() > 0)
    {
        char command = bluetooth.read();

        if (command == 'R')
        {
            openRecycle();
        }
        else if (command == 'N')
        {
            openNonRecycle();
        }
        else if (command == 'X')
        {
            //nothing optional.
        }
    }
}