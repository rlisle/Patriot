/*******************************************************
Fan Example

This example shows how to create a temperature controlled fan

 * Fan     DAC     Photon pin 6

It assumes that a temperature device will broadcast events
named "InsideTemp"

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-25: Update for IoT v1.2.0. Add off behavior.
2017-05-19: Create fan plugin library
********************************************************/
#include <IoT.h>
#include <PatriotFan.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    // Create device
    Fan *fan = new Fan(DAC, "Porch");


    // Add it
    iot->addDevice(fan);

    // Setup behaviors to control fan based on temperature
    iot->addBehavior(new Behavior(fan, "InsideTemp", '>', 75, 10));
    iot->addBehavior(new Behavior(fan, "InsideTemp", '>', 80, 50));
    iot->addBehavior(new Behavior(fan, "InsideTemp", '>', 85, 100));
    iot->addBehavior(new Behavior(fan, "InsideTemp", '<', 76, 0));
}

void loop() {
    iot->loop();
}
