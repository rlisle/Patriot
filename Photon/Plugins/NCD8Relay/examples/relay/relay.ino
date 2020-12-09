/**
NCD 8 Relay board Example

This example supports the 1st relay on an NCD 8 Relay board.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/

#include <IoT.h>
#include <PatriotNCD8Relay.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    byte address = 0x20;
    byte numRelays = 8;
    Relay *relay1 = new Relay(address, numRelays, 0, "relay");
    iot->addDevice(relay1);

    iot->addBehavior(relay1, new Behavior("my gadget", '>', 0, 100));    // Turn on
    iot->addBehavior(relay1, new Behavior("my gadget", '=', 0, 0));      // Turn off
}

void loop() {
    iot->loop();
}
