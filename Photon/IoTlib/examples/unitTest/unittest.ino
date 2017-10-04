/******************************************************************
UnitTest sketch for Patriot library

This sketch is needed by the iOS and Mocha unit tests.
It is typically flashed to the UnitTest device using the web console.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-04-03: Created based on starter.ino
******************************************************************/

#include <IoT.h>
#include <PatriotLight.h>

IoT *iot;

// Forward reference
int testFunc(String data);

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    iot->log("Test sketch started");

    Light *light = new Light(D7, "Led");  // This is the small blue LED on a photon
    iot->addDevice(light);

    iot->addBehavior(new Behavior(light, "photon", '>', 0, 100));

    iot->exposeActivities();
    iot->exposeControllers();

    // Test that user functions work
    Particle.function("test", testFunc);
}

void loop() {
    iot->loop();
}

int testFunc(String data)
{
    iot->log("Test function called with string: "+data);
    return 55;
}
