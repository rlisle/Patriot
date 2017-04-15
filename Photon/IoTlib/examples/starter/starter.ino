/******************************************************************
Example starting point sketch for Patriot library

This example uses the built-in blue LED only.
It is provided as a quick start for your own sketches.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-28: Use fixed 'patriot' event name.
2017-03-24: Initial creation
******************************************************************/

#include <IoT.h>
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("photon");  // The name of your Photon device. Can be whatever you named it.
    iot->begin();

    //Note: D7 is not a PWM pin, so need to using digitalWrite instead of analogWrite
    iot->addLight(D7, "Led");               // This is the small blue LED on a photon

    // Alexa will respond to "Alexa, turn photon on" or "Alexa, turn on photon"
    // You can change the word 'photon' to whatever you like, but it needs to be something
    // that Alexa can recognize. For now, use a single word.
    iot->addBehavior("Led", new Behavior("photon",'>',0, 100));

    iot->monitorPressure("Room", 30000);
}

void loop() {
    iot->loop();
}
