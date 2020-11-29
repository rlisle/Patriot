/*******************************************************
NCD 16 Switch board Example

This example supports a switch connected to the 1st input
on an NCD 16 GPIO board.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-28: Initial creation (based on NCD8Switch)
********************************************************/

#include <IoT.h>
#include <PatriotNCD16Switch.h>

String mqttServerIP = "192.168.10.184"; // Set this to your MQTT broker IP

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("MyTest");
    iot->begin();
    iot->connectMQTT(mqttServerIP, "myPatriotTest1");   // Whatever you want here

    byte address = 0x00;    // No jumpers set
    Switch *switch1 = new Switch(address, 0, "switch1");
    iot->addDevice(switch1);
}

void loop() {
    iot->loop();
}
