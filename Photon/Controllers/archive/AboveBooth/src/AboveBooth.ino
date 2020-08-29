/*
 * AboveBooth Controller
 * Description: This sketch controls the lights above the booth and piano.
 * Author: Ron Lisle
 * Date: 3/15/17
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoT v2 PCB
 * 3. 4 x LED driver boards (left to right at top):
 *    - Piano spots     (A3)
 *    - Overhead lights (RX)
 *    - Booth spots     (TX)
 *    - Booth lamp      (WKP)
 *
 * ToDo:
 * 1. Booth proximity detector  ?
 * 2. Piano proximity detector  ?
 * 3. Motion detector           (A5)
 * 4. Light Sensor              (A4)
 *
 * History
 *  5/27/17 Update to use plugins
 *  3/30/17 Remove event name per v1.0 fixed 'patriot' event name
 *  3/15/17 Convert to IoT library
 *  1/11/17 Updated to auto registration
 */
#include <IoT.h>
#include <PatriotLight.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("AboveBooth");
    iot->setPublishName("patriot");
    iot->begin();

    Light *boothSpots = new Light(TX, "BoothSpots");
    Light *boothLamp = new Light(WKP, "boothlamp");
    Light *pianoSpots = new Light(RX, "pianospots");
    Light *overhead = new Light(A3, "overhead");

    iot->addDevice(boothSpots);
    iot->addDevice(boothLamp);
    iot->addDevice(pianoSpots);
    iot->addDevice(overhead);

    iot->addBehavior(new Behavior(pianoSpots, "piano", '>', 0, 100));
    iot->addBehavior(new Behavior(boothLamp, "piano", '>', 0, 10));
    iot->addBehavior(new Behavior(boothLamp, "booth", '>', 0, 100));
    iot->addBehavior(new Behavior(boothSpots, "booth", '>', 0, 100));
    iot->addBehavior(new Behavior(boothSpots, "computer", '>', 0, 10));
    iot->addBehavior(new Behavior(pianoSpots, "computer", '>', 0, 10));
    iot->addBehavior(new Behavior(boothLamp, "away", '>', 0, 0));
    iot->addBehavior(new Behavior(boothSpots, "away", '>', 0, 0));
    iot->addBehavior(new Behavior(pianoSpots, "away", '>', 0, 0));
    iot->addBehavior(new Behavior(overhead, "away", '>', 0, 0));
}

void loop() {
    iot->loop();
}
