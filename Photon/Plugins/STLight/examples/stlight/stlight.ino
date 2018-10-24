/*******************************************************
STLights Example

This example contains 3 Smartthings light bulbs
    - Desk Lamp
    - Kitchen Lamp
    - Bedroom Lamp

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2018-10-15: Initial creation from modified copy of light.ino
********************************************************/

#include <IoT.h>
#include <PatriotSTLight.h>

IoT *iot;

byte mqttServerIPAddress[] = {192.168.10.101};

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("MyController");
    iot->begin();
    iot->connectMQTT(mqttServerIPAddress, "myController1");

    // Create your devices
    STLight *deskLamp = new STLight("Desk Lamp", "Desk Lamp");
    STLight *kitchenLamp = new STLight("Kitchen Lamp", "Kitchen Lamp");
    STLight *bedroomLamp = new STLight("Bedroom Lamp", "Bedroom Lamp");

    // Add them to IoT
    iot->addDevice(deskLamp);
    iot->addDevice(kitchenLamp);
    iot->addDevice(bedroomLamp);

    // Add any behaviors if appropriate
    iot->addBehavior(new Behavior(deskLamp, "Desk Work", '>', 0, 100));
    iot->addBehavior(new Behavior(deskLamp, "Desk Work", '=', 0, 0));
    iot->addBehavior(new Behavior(kitchenLamp, "Cook", '>', 0, 100));
    iot->addBehavior(new Behavior(kitchenLamp, "Cook", '=', 0, 0));
    iot->addBehavior(new Behavior(deskLamp, "Read", '>', 0, 100));
    iot->addBehavior(new Behavior(deskLamp, "Read", '=', 0, 0));
}

void loop() {
    iot->loop();
}