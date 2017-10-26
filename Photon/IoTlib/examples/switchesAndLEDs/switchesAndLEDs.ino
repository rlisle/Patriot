/*******************************************************
Switches and LEDs Patriot Example

This example requires some breadboarding.
It uses 4 LEDs and 4 switch to demonstrate the more complex
capabilites when using multiple devices.

You can use a jumper wire to connect D0 to ground to turn the
LED on and off, or use the iOS app or Alexa.
 * 4 LED Lights
     - Red   = Outside      A3      Photon pin 9
     - Green = Kitchen      RX      Photon pin 4
     - Blue  = Bedroom      TX      Photon pin 3
     - Amber = Bathroom     WKP     Photon pin 5

 * 4 Switches
     - WakeUp               D4      Photon pin 17
     - WatchTV              D5      Photon pin 18
     - Cook                 D6      Photon pin 19
     - Sleep                D7      Photon pin 20

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-26: Update for v2.
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
    Light *outside = new Light(A3, "Outside");
    Light *kitchen = new Light(RX, "Kitchen");
    Light *living = new Light(TX, "Livingroom");
    Light *bedroom = new Light(WKP, "Bedroom");

    Switch *switch1 = new Switch(D4, "WakeUp");
    Switch *switch2 = new Switch(D5, "WatchTV");
    Switch *switch3 = new Switch(D6, "Cook");
    Switch *switch4 = new Switch(D7, "Sleep");

    // Add them
    iot->addDevice(outside);
    iot->addDevice(kitchen);
    iot->addDevice(living);
    iot->addDevice(bedroom);

    iot->addDevice(switch1);
    iot->addDevice(switch2);
    iot->addDevice(switch3);
    iot->addDevice(switch4);

    // Setup behaviors for our devices
    // Behaviors can be named whatever you like, and can control multiple devices.
    iot->addBehavior(new Behavior(outside, "WakeUp", '>', 0, 100));
    iot->addBehavior(new Behavior(kitchen, "WatchTV", '>', 0, 100));
    iot->addBehavior(new Behavior(living, "Cook", '>', 0, 100));
    iot->addBehavior(new Behavior(bedroom, "Sleep",'>',0, 100));
}

void loop() {
    iot->loop();
}
