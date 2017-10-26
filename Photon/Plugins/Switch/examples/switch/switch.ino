/*******************************************************
Patriot Switch Example

This example shows how to use the Patriot Switch plugin
It includes:

 * 4 Switches
    - Wake                  D4      Photon pin 17
    - TV                    D5      Photon pin 18
    - Cook                  D6      Photon pin 19
    - Sleep                 D7      Photon pin 20

When a switch is turned on, the command <name>:100 will be
published. When the switch is turned off, it will publish
the command <name>:0.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-25: Update for IoT v1.2.0. Add additional notes.
2017-05-19: Initial creation
********************************************************/

#include <IoT.h>
#include <PatriotSwitch.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->setPublishName("patriot");
    iot->begin();

    // Create 4 switches
    Switch *switch1 = new Switch(D4, "Wake");
    Switch *switch2 = new Switch(D5, "TV");
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
