/*******************************************************
Patriot Checklist Example

This example shows how to use the Patriot Checklist plugin

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2021-10-27: Initial creation
********************************************************/

#include <IoT.h>
#include <PatriotChecklist.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Create checklist
    Checklist *checklist1 = new Checklist("Start List");

    // Add it to IoT
    iot->addDevice(checklist1);
}

void loop() {
    iot->loop();
}
