/**
NCD 8 Light board Example

This example supports the 1st channel on an NCD 8 PWM board.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/

#include <IoT.h>
#include <PatriotNCD8Light.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    byte address = 0x20;
    byte numLights = 8;
    iot->addDevice(new NCDLight(address, numLights, 0, "light1"));
}

void loop() {
    iot->loop();
}
