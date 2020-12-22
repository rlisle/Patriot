/**
Lights Example

This example contains 4 lights

 * 4 LED Lights
    - Red   = Porch         A3      Photon pin 9
    - Green = Table         RX      Photon pin 4
    - Blue  = Reading       TX      Photon pin 3
    - Amber = Sink          WKP     Photon pin 5

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/

#include <IoT.h>
#include <PatriotLight.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->setPublishName("patriot");
    iot->begin();

    // Create devices
    Light *porch = new Light(A3, "Porch");
    Light *table = new Light(RX, "Table");
    Light *reading = new Light(TX, "Reading");
    Light *sink = new Light(WKP, "Sink");

    // Add local control switch pins D0 - D3
    // These are simply tied to ground, normally open
    porch->setLocalPin(D0, "Porch Switch");
    porch->setLocalPin(D1, "Table Switch");
    porch->setLocalPin(D2, "Reading Switch");
    porch->setLocalPin(D3, "Sink Switch");

    // Add them
    iot->addDevice(porch);
    iot->addDevice(table);
    iot->addDevice(reading);
    iot->addDevice(sink);
}

void loop() {
    iot->loop();
}
