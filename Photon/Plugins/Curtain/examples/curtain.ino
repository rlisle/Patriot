/**
NCD 4 Relay Curtain Example

This example supports the 1st relay on an NCD 4 Relay GPIO board.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/

#include <IoT.h>
#include <PatriotCurtain.h>

void setup() {
    IoT::begin("192.168.10.184", "RonTest");

    Device::add(new Curtain(0, 1, "curtains"));

}

void loop() {
    IoT::loop();
}
