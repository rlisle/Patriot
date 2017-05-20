/*******************************************************
Patriot Switch Example

This example shows how to use the Patriot Switch plugin
It includes:

 * 4 Switches
    - WakeUp                D4      Photon pin 17
    - WatchTV               D5      Photon pin 18
    - Cook                  D6      Photon pin 19
    - Sleep                 D7      Photon pin 20

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-19: Initial creation
********************************************************/

#include <IoT.h>
#include <PatriotSwitch.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    // Create 4 switches
    Switch *switch1 = new Switch(D4, "WakeUp");
    Switch *switch2 = new Switch(D5, "WatchTV");
    Switch *switch3 = new Switch(D6, "Cook");
    Switch *switch4 = new Switch(D7, "Sleep");

    // Add them to IoT
    iot->addDevice(switch1);
    iot->addDevice(switch2);
    iot->addDevice(switch3);
    iot->addDevice(switch4);
}

void loop() {
    iot->loop();
}
