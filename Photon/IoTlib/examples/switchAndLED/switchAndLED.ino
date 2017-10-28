/*******************************************************
Example switch and LED Patriot sketch

This example uses the built-in blue LED and a switch on D0.
You can use a jumper wire to connect D0 to ground to turn the
LED on and off, or use the iOS app or Alexa.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-25: Update for IoT v2. Add OFF behaviors.
2017-03-28: Use fixed 'patriot' event name.
2017-03-24: Rename Patriot
2017-03-11: Created switch & leds
2017-03-07: Initial creation
********************************************************/

#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotSwitch.h>

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

    // The name provided to the switch is the name that will be published.
    // So give it the name corresponding with either a device to be controlled,
    // or better yet a behavior name. In this case, the switch will control the
    // behavior named "photon".
    Switch *switch1 = new Switch(D0, "photon");

    // Tell IoT about the devices you defined above
    iot->addDevice(light1);
    iot->addDevice(switch1);

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
