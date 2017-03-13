/*******************************************************
Switches and LEDs ParticleIoT Example

This example contains 4 switches and 4 LEDs:

 * 4 LED Lights
     - Red   = Outside      A3      Photon pin 9
     - Green = Kitchen      RX      Photon pin 4
     - Blue  = Bedroom      TX      Photon pin 3
     - Amber = Bathroom     WKP     Photon pin 5

 * 4 Switches
     - WakeUp               D4      Photon pin 17
     - WatchTV              D5      Photon pin 18
     - Cook                 D6      Photon pin 19
     - Sleep                D7      Photon pin 20

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-11: Created switch & leds
2017-03-07: Initial creation
********************************************************/

#include <IoT.h>
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("ParticleIoT");
    iot->setPublishName("RonsRV");
    iot->begin();

    iot->addLight(A3, 'Outside');
    iot->addLight(RX, 'Kitchen');
    iot->addLight(TX, 'Livingroom');
    iot->addLight(WKP, 'Bedroom');

    iot->addSwitch(D4, 'WakeUp');
    iot->addSwitch(D5, 'WatchTV');
    iot->addSwitch(D6, 'Cook');
    iot->addSwitch(D7, 'Sleep');

    iot->addBehavior('Outside', new Behavior('WakeUp','>',0, 100));
    iot->addBehavior('Kitchen', new Behavior('WatchTV','>',0, 100));
    iot->addBehavior('Livingroom', new Behavior('Cook','>',0, 100));
    iot->addBehavior('Bedroom', new Behavior('Sleep','>',0, 100));

    iot->exposeControllers();
    iot->exposeActivities();
}

void loop() {
    iot->loop(millis());
}
