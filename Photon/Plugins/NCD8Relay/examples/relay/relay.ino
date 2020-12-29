/**
NCD 8 Relay board Example

This example supports the 1st relay on an NCD 8 Relay board.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/

#include <IoT.h>
#include <PatriotNCD8Relay.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("MyPhoton");
    iot->begin();
    iot->connectMQTT(mqttServer, "PatriotPhoton1");

    byte address = 0x20;
    byte numRelays = 8;
    iot->addDevice(new Relay(address, numRelays, 0, "relay"));
}

void loop() {
    iot->loop();
}
