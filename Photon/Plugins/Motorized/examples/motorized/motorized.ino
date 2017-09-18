/*******************************************************
Motorized Example

This example shows how to create a motorized vent

 * Vent1     D7     Photon pin 7

It assumes that a temperature device will broadcast events
named "InsideTemp". The vent behavior will open the vent
 when InsideTemp is greater than 85, and close it when
 InsideTemp drops below 80.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-09-17: Create motorized plugin library
********************************************************/
#include <IoT.h>
#include <PatriotMotorized.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    // Create device
    Motorized *vent1 = new Motorized(D7, "Vent1");


    // Add it
    iot->addDevice(vent1);

    // Setup behaviors to control vent based on temperature
    iot->addBehavior(new Behavior(vent1, "InsideTemp", '>', 85, 100));
    iot->addBehavior(new Behavior(vent1, "InsideTemp", '<', 80, 0));
}

void loop() {
    iot->loop();
}
