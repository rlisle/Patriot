/**
NCD 4 Relay board Example

This example supports the 3rd relay on an NCD 4 Relay + 4 GPIO board.

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
    iot->addDevice(new Relay(address, 0, "relay"));
}

void loop() {
    iot->loop();
}
