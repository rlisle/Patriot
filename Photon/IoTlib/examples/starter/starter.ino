/******************************************************************
Example starting point sketch for Patriot library

This example uses the built-in LED only.
It is provided as a quick start for your own sketches.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Initial creation
******************************************************************/

#include <IoT.h>
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myController");
    iot->setPublishName("myEvent");         // Set all devices to the same event name
    iot->begin();

    iot->addLight(D7, 'Led');               // This is the small blue LED on a photon
    // Alexa will repsond to "Alexa, turn lights on" or "Alexa, turn on lights"
    iot->addBehavior('Led', new Behavior('Lights','>',0, 100));
}

void loop() {
    iot->loop();
}
