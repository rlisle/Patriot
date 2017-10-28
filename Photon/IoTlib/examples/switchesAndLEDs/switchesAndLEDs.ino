/*******************************************************
Switches and LEDs Patriot Example

This example requires some breadboarding.
It uses 4 LEDs and 4 switch to demonstrate the more complex
capabilites when using multiple devices and behaviors.

 * 4 LED Lights
     - Red   = Outside      D0      Photon pin 13
     - Green = Kitchen      D1      Photon pin 14
     - Blue  = Living Room  D2      Photon pin 15
     - Amber = Bedroom      D3      Photon pin 16

You can use a switch to ground or a jumper wire to connect
any of the switch pins A0, A1, A2, A3 to ground to control
the devices and behaviors.

 * 4 Switches
     - Outside              A0      Photon pin 12
     - Wake up              A1      Photon pin 11
     - Watch TV             A2      Photon pin 10
     - Sleep                A3      Photon pin 9

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-10-26: Update for v2. Change pins and names for clarity.
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
    // The name of the light is the name used to control it.
    // So for example, saying "Alexa, turn on kitchen" will
    // turn on the LED connected to pin D1.
    Light *outside = new Light(D0, "Outside");
    Light *kitchen = new Light(D1, "Kitchen");
    Light *living = new Light(D2, "Living Room");
    Light *bedroom = new Light(D3, "Bedroom");

    // The name listed for a switch is not the switch's name.
    // It is the command that is issued when the switch is pressed.
    Switch *switch1 = new Switch(A0, "Outside");
    Switch *switch2 = new Switch(A1, "Wake Up");
    Switch *switch3 = new Switch(A2, "Watch TV");
    Switch *switch4 = new Switch(A3, "Sleep");

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
    // Note: do not use the same name as a device.
    iot->addBehavior(new Behavior(outside, "Wake Up", '>', 0, 0));    // Wake up turns off outside lights
    iot->addBehavior(new Behavior(bedroom, "Wake Up", '>', 0, 100));  // and turns on the bedroom lights

    iot->addBehavior(new Behavior(kitchen, "Watch TV", '>', 0, 50));  // Watch TV "on" sets living room lights to 50%
    iot->addBehavior(new Behavior(kitchen, "Watch TV", '=', 0, 0));   // Watch TV "off" turns living room lights off

    iot->addBehavior(new Behavior(bedroom, "Sleep",'>',0, 0));      // Sleep turns off bedroom lights
    iot->addBehavior(new Behavior(kitchen, "Sleep",'>',0, 0));      // and kitchen lights
    iot->addBehavior(new Behavior(living, "Sleep",'>',0, 0));       // and living room lights
    iot->addBehavior(new Behavior(outside, "Sleep",'>',0, 100));    // but turns ON outside lights
}

void loop() {
    iot->loop();
}
