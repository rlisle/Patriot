/*******************************************************
Patriot MR24 Example

This example shows how to use the Patriot MR24 plugin
It includes:

When motion is detected, the command <name>:100 will be published.
 
When Human Presence is detected, the command <name>:50 will be
published.
 
When not detected, it will publish the command <name>:0.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2022-02-19: Initial creation by modifying PIR plugin.
********************************************************/

#include <IoT.h>
#include <MR24.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Create PIR sensor
    MR24 *mr24 = new MR24(D4, D5, "HumanDetected");

    // Add it to IoT
    iot->addDevice(mr24);
}

void loop() {
    iot->loop();
}
