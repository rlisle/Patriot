/**
NCD 16 12-bit Dimmer board Example

This example supports the 1st channel on an NCD 16 12-bit PWM board.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/

#include <IoT.h>
#include <PatriotNCD16Dimmer.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    byte address = 0x20;
    byte numLights = 16;
    iot->addDevice(new NCD16Dimmer(address, 0, "light1"));
}

void loop() {
    iot->loop();
}
