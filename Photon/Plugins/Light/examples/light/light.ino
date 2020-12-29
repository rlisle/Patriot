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

String mqttServer = "192.168.10.184";

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();
    iot->connectMQTT(mqttServer, "PatriotRearPanel1", true);

    // Define devices
    iot->addDevice(new Light(A3, "Porch"));
    iot->addDevice(new Light(RX, "Table"));
    iot->addDevice(new Light(TX, "Reading"));
    iot->addDevice(new Light(WKP, "Sink"));
}

void loop() {
    iot->loop();
}
