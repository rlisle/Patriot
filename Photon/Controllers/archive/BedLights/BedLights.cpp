/*
 * Bed Lights Controller
 *
 * This sketch controls the lights above the bed.
 *
 * 1. Monitor subscribes
 * 2. Adjust lights
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoT
 * 3. 3 x LED driver boards:
 *    - Left bed spot  (TX)
 *    - Center bed light (?)
 *    - Right bed spot (RX)
 *
 * ToDo:
 * 4. left and right proximity detectors
 * 5. Motion detector
 * 6. Light Sensor
 */

/****************/
/*** INCLUDES ***/
/****************/
#include "application.h"
#include "iot.h"

/*****************/
/*** CONSTANTS ***/
/*****************/
#define kLeftBedSpotsPin    kLed2Pin // RX
#define kCenterBedLightsPin kLed3Pin // TX
#define kRightBedSpotsPin   kLed4Pin // WKP

/*****************/
/*** VARIABLES ***/
/*****************/
IoT*      iot;

/*************/
/*** SETUP ***/
/*************/
void setup() {

  iot = IoT::getInstance();
  iot->start("BedLights");

  setupLights();
  setupBehaviors();

  //  iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, "couch left");

}

void setupLights() {
  iot->addLight(kLeftBedSpotsPin, kBedLeftSpots);
  iot->addLight(kCenterBedLightsPin, kBedCenterLights);
  iot->addLight(kRightBedSpotsPin, kBedRightSpots);
}

void setupBehaviors() {
  iot->addBehavior(kBedLeftSpots, new Behavior(kBedLeftReading,'>',0, 10));
  iot->addBehavior(kBedRightSpots, new Behavior(kBedRightReading,'>',0, 10));
  iot->addBehavior(kBedCenterLights, new Behavior(kBedLightsActivity,'>',0, 100));
  iot->addBehavior(kBedLeftSpots, new Behavior(kAllLightsActivity,'>',0, 100));
  iot->addBehavior(kBedRightSpots, new Behavior(kAllLightsActivity,'>',0, 100));
  iot->addBehavior(kBedCenterLights, new Behavior(kAllLightsActivity,'>',0, 100));
}

/************/
/*** LOOP ***/
/************/
void loop() {
  iot->loop();
}
