/*******************************************************
Relay Example

This example supports a relay connected to pin D7.
D7 is also connected to the built-in blue LED, so
it can be used to view operation.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-31: Initial creation
********************************************************/

#include <IoT.h>
#include <PatriotRelay.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Define our "relay" device on pin D7
    // Since no duration is specified, it is set to a
    // default value of 0 meaning no automatic turn off.
    Relay *relay = new Relay(D7, "relay");
    iot->addDevice(relay);
}

void loop() {
    iot->loop();
}
