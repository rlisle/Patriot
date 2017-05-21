/*******************************************************
DHT Example

This example contains 1 DHT sensor connected to pin D4

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-20: Initial DHT plugin creation.
********************************************************/

#include <IoT.h>
#include <PatriotDHT.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();
    iot->addDevice(new DHT(D4, DHT11));
}

void loop() {
    iot->loop();
}
