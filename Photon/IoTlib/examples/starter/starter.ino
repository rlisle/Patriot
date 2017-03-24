/******************************************************************
Example starting point sketch for ParticleIoT library

This example uses the built-in LED only.
It is provided as a quick start for your own sketches.

http://www.github.com/rlisle/Patriot

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
    iot->setPublishName("RonsRV");         // Set all devices to the same event name
    iot->begin();

    iot->addLight(D7, 'Led');
    // Alexa will repsond to "Alexa, turn lights on" or "Alexa, turn on lights"
    iot->addBehavior('Led', new Behavior('Lights','>',0, 100));
}

void loop() {
    iot->loop();
}
