/*******************************************************
Patriot HC-SR04 Ultrasonic Proximity Sensor Example

This example shows how to use the Patriot Switch plugin
It includes:

 * HC-SR04 Ultrasonic Proximity Sensor
    - Trigger               A1      Photon pin 11
    - Echo                  A0      Photon pin 12

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-21: Initial creation
********************************************************/

#include <IoT.h>
#include <PatriotUltrasonic.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    // Create the HC-SR04 sensor device
    Ultrasonic *ultra1 = new Ultrasonic(A1, A0, "Proximity");

    // Add it to IoT
    iot->addDevice(ultra1);
}

void loop() {
    iot->loop();
}
