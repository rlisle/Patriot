/******************************************************************
Example starting point sketch for Patriot library

This example uses the built-in blue LED.
You can use the Patriot iOS app or Alexa or the Alexa app to turn
the LED on or off.

It is provided as a quick start for your own sketches.
Refer also to the examples included with each Patriot plugin library.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-25: Update for IoT v2. Add OFF behaviors.
            Remove switch. Refer to the SwitchAndLED example instead.
2017-03-28: Use fixed 'patriot' event name.
2017-03-24: Initial creation
******************************************************************/

#include <IoT.h>
#include <PatriotLight.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Create devices
    //Note: D7 is not a PWM pin, so it can only turn on or off
    // Alexa will respond to "Alexa, turn L E D on" or "Alexa, turn off L E D"
    // You can change the name 'LED' to whatever you like, but it needs to be something
    // that Alexa can recognize.
    Light *light1 = new Light(D7, "LED");

    // Tell IoT about the devices you defined above
    iot->addDevice(light1);

    // The "behavior" defines additional commands that control things.
    // Note that the name of each device can be used to control it, so behaviors are
    //      not required unless you want to control more than one device with a commands.
    // Alexa will respond to "Alexa, turn photon on" or "Alexa, turn off photon"
    // You can change the word 'photon' to whatever you like, but it needs to be something
    // that Alexa can recognize. For now, use a single word.
    iot->addBehavior(new Behavior(light1, "photon", '>', 0, 100));      // On
    iot->addBehavior(new Behavior(light1, "photon", '=', 0, 0));        // Off
}

void loop() {
    iot->loop();
}
