/*******************************************************
Patriot DateTime Example

This example shows how to use the Patriot DateTime plugin

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

********************************************************/

#include <IoT.h>
#include <PatriotDateTime.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Create DateTime device
    DateTime *dateTime = new DateTime();

    // Add it to IoT
    iot->addDevice(dateTime);
}

void loop() {
    iot->loop();
}
