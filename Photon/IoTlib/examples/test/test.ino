/******************************************************************
Test sketch for Patriot library

This sketch is needed by the iOS unit tests.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-04-03: Created based on starter.ino
******************************************************************/

#include <IoT.h>
IoT *iot;

// Forward reference
int testFunc(String data);

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    iot->log("Test sketch started");

    iot->addLight(D7, "Led");               // This is the small blue LED on a photon
    iot->addBehavior("Led", new Behavior("photon",'>',0, 100));
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
