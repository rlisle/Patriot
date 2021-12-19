/*******************************************************
Patriot PIR Example

This example shows how to use the Patriot PIR plugin
It includes:

When a PIR is turned on, the command <name>:100 will be
published.
When the switch is turned off, it will publish
the command <name>:0.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2021-12-18: Initial creation by modifying Switch plugin.
********************************************************/

#include <IoT.h>
#include <PatriotPIR.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Create PIR sensor
    PIR *pir1 = new PIR(D0, "Movement");

    // Add it to IoT
    iot->addDevice(pir1);
}

void loop() {
    iot->loop();
}
