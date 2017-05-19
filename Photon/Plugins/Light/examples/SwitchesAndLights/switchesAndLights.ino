/*******************************************************
Switches and Lights Patriot Example

This example contains 4 switches and 4 lights

 * 4 LED Lights
    - Red   = Outside       A3      Photon pin 9
    - Green = Kitchen       RX      Photon pin 4
    - Blue  = Bedroom       TX      Photon pin 3
    - Amber = Bathroom      WKP     Photon pin 5

 * 4 Switches
    - WakeUp                D4      Photon pin 17
    - WatchTV               D5      Photon pin 18
    - Cook                  D6      Photon pin 19
    - Sleep                 D7      Photon pin 20

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-19: Convert to plugin libraries
2017-03-28: Use fixed 'patriot' event name.
2017-03-24: Rename Patriot
2017-03-11: TODO: add remaining devices.
2017-03-07: Initial creation
********************************************************/

#include <IoT.h>
#include <PatriotSwitch.h>
#include <PatriotLight.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    // Create devices
    Switch *switch1 = new Switch(D4, "WakeUp");
    Switch *switch2 = new Switch(D5, "WatchTV");
    Switch *switch3 = new Switch(D6, "Cook");
    Switch *switch4 = new Switch(D7, "Sleep");

    Light *outside = new Light(A3, "Outside");
    Light *kitchen = new Light(RX, "Kitchen");
    Light *living = new Light(TX, "Livingroom");
    Light *bedroom = new Light(WKP, "Bedroom");


    // Add them
    iot->addDevice(switch1);
    iot->addDevice(switch2);
    iot->addDevice(switch3);
    iot->addDevice(switch4);

    iot->addDevice(outside);
    iot->addDevice(kitchen);
    iot->addDevice(living);
    iot->addDevice(bedroom);

    // Setup behaviors for our devices
    iot->addBehavior(new Behavior(outside, "WakeUp", '>', 0, 100));
    iot->addBehavior(new Behavior(kitchen, "WatchTV", '>', 0, 100));
    iot->addBehavior(new Behavior(living, "Cook", '>', 0, 100));
    iot->addBehavior(new Behavior(bedroom, "Sleep",'>',0, 100));
}

void loop() {
    iot->loop();
}
