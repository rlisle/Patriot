/*******************************************************
Timed Relay Example

This example supports a relay connected to pin D7.
It will automatically turn off the relay 5 seconds
after the relay is turned on.

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

    // Define our "relay" device on pin D7 with 5 second duration
    Relay *relay = new Relay(D7, "relay", 5);
    iot->addDevice(relay);
}

void loop() {
    iot->loop();
}
