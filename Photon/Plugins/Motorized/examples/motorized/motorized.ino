/*******************************************************
Motorized Test

This sketch will exercise the Motorized plugin.
For an example of how to use the motorized plugin in
Patriot IoT, refer to the IoT examples/motorized.

D7 is used for both the open and close operations
so it can be observed using the blue LED.
Normally separate I/O pins would be used.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-02: Convert to a test sketch. Example moved to IoT examples
********************************************************/
#include <PatriotMotorized.h>

Motorized *vent;

void setup() {
    // Create device using D7 for both open and closed. Run for 5 seconds.
    vent = new Motorized(D7, D7, 5000, "Vent1");

    vent->setPercent(100);
}

void loop() {
    vent->loop();
}
