/*******************************************************
Patriot Switch Example

This example shows how to use the Patriot Switch plugin
It includes:

When a switch is turned on, the command <name>:100 will be
published.
When the switch is turned off, it will publish
the command <name>:0.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-25: Update for IoT v2.0.0. Add additional notes.
2017-05-19: Initial creation
********************************************************/

#include <IoT.h>
#include <PatriotSwitch.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Create switch
    Switch *switch1 = new Switch(D0, "Wake");

    // Add it to IoT
    iot->addDevice(switch1);
}

void loop() {
    iot->loop();
}
