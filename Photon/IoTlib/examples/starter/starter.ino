/******************************************************************
Example starting point sketch for Patriot library

This example uses the built-in blue LED and a switch on D0.
You can use a jumper wire to connect D0 to ground to turn the
LED on and off, or use the iOS app or Alexa.

It is provided as a quick start for your own sketches.
Alternatively, you can use the Everything example, and delete
what you don't need.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-28: Use fixed 'patriot' event name.
2017-03-24: Initial creation
******************************************************************/

#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotSwitch.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton"); // The name of your Photon device. Can be whatever you named it.
    iot->begin();

    // Create devices
    //Note: D7 is not a PWM pin, so it can only turn on or off
    Light *light1 = new Light(D7, "Led");
    Switch *switch1 = new Switch(D0, "PushMe");

    // Tell IoT about the devices you defined above
    iot->addDevice(light1);
    iot->addDevice(switch1);

    // The "behavior" defines the commands that control things.
    // Alexa will respond to "Alexa, turn photon on" or "Alexa, turn off photon"
    // You can change the word 'photon' to whatever you like, but it needs to be something
    // that Alexa can recognize. For now, use a single word.
    iot->addBehavior(new Behavior(light1, "photon", '>', 0, 100));
}

void loop() {
    iot->loop();
}
