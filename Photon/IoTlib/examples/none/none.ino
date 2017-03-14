/******************************************************************
Example starting point sketch for ParticleIoT library

This example contains no devices.
It is provided as a starting point for you own sketches.

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-11: Initial creation
******************************************************************/

#include <IoT.h>
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("ParticleIoT");
    iot->setPublishName("RonsRV");
    iot->begin();
}

void loop() {
    iot->loop(millis());
}
