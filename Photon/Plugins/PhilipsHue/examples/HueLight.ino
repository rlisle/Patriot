/*******************************************************
Patriot Philips Hue Example

This example shows how to use the Patriot Philips Hue plugin

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2021-10-27: Initial creation
********************************************************/

#include <IoT.h>
#include <HueLight.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Create checklist
    HueLight *hueLight = new HueLight("Desk Lamp");

    // Add it to IoT
    iot->addDevice(hueLight);
}

void loop() {
    iot->loop();
}
