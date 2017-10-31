/*******************************************************
Relay Example

This example supports the 1st relay on an NCD 8 Relay board.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-03: Initial creation
********************************************************/

#include <IoT.h>
#include <PatriotNCD8Relay.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->setPublishName("patriot");
    iot->begin();

    byte address = 0x20;
    byte numRelays = 8;
    Relay *relay1 = new Relay(address, numRelays, 0, "relay1");
    iot->addDevice(relay1);

    iot->addBehavior(relay1, new Behavior("Relay", '>', 0, 100));
}

void loop() {
    iot->loop();
}
