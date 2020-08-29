/*
 * Couch Lights Controller
 *
 * This sketch controls the lights above the couch.
 *
 * 1. Monitor subscribes
 * 2. Adjust lights
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoT
 * 3. 2 x LED driver boards:
 *    - Left couch spot  (TX)
 *    - Right couch spot (RX)
 *
 * ToDo:
 * 1. left and right proximity detectors
 * 3. Motion detector
 * 4. Light Sensor
 *
 * History
 *  3/30/17 Remove event name per v1.0 fixed 'patriot' event name
 *  3/23/17 Convert to IoT library
 */

#include <IoT.h>
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("CouchLights");
    iot->setPublishName("patriot");
    iot->begin();

    iot->addLight(TX, "Couch left spots");
    iot->addLight(TX, "Couch right spots");

    iot->addBehavior("Couch right spots", new Behavior("Ron Reading", '>', 0, 100));
    iot->addBehavior("Couch left spots", new Behavior("Shelley Reading", '>', 0, 100));

    iot->addBehavior("Couch right spots", new Behavior("Watch TV", '>', 0, 10));
    iot->addBehavior("Couch left spots", new Behavior("Watch TV", '>', 0, 10));

    iot->addBehavior("Couch right spots", new Behavior("Away", '>', 0, 0));
    iot->addBehavior("Couch left spots", new Behavior("Away", '>', 0, 0));

    iot->addBehavior("Couch right spots", new Behavior("Sleep", '>', 0, 0));
    iot->addBehavior("Couch left spots", new Behavior("Sleep", '>', 0, 0));
}

void loop() {
    iot->loop();
}
