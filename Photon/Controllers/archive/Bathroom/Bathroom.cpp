/*
 * Bathroom Controller
 *
 * This sketch controls the bathroom sink lights and switches.
 * It handles 4 wall switches located by the front door.
 * Optionally, it will support temperature & humidity.
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoT
 * 3. ? wall switches
 * 5. ? LED driver boards:
 * 9. DHT11             (Tx, Rx)
 */

/****************/
/*** INCLUDES ***/
/****************/
#include "application.h"
#include "iot.h"

/*****************/
/*** CONSTANTS ***/
/*****************/
#define kSwitch1Pin             D4
#define kSwitch2Pin             D5
#define kSwitch3Pin             D6
#define kSwitch4Pin             D7
#define kMainLightsPin          WKP
#define kOutsideLightsPin       A5
#define kCounterLightsPin       A4
#define kInsideMotionPin        D2
#define kOutsideMotionPin       D3
#define kInsideLightSensorPin   A0
#define kOutsideLightSensorPin  A1
#define kDoorBellSwitchPin      A2
#define kInsideDHTPin           TX
#define kOutsideDHTPin          RX

/*****************/
/*** VARIABLES ***/
/*****************/
IoT*      iot;

/*************/
/*** SETUP ***/
/*************/
void setup() {

  iot = IoT::getInstance();
  iot->start("Bathroom");

  setupLights();
  setupSwitches();
  setupBehaviors();

//  iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, "couch left");
}

void setupLights() {
  // iot->addLight(kOutsideLightsPin, kOutsideLights);
  // iot->addLight(kMainLightsPin, kMainLights);
  // iot->addLight(kCounterLightsPin, kCounterSpots);
}

void setupSwitches() {
  // iot->addSwitch(kSwitch4Pin,kKitchenActivity); // Right-most switch
  // iot->addSwitch(kSwitch3Pin,kCoffeeActivity);
  // iot->addSwitch(kSwitch2Pin,kOverheadActivity);
  // iot->addSwitch(kSwitch1Pin,kOutsideActivity);    // Left-most switch
}

void setupBehaviors() {
  // iot->addBehavior(kOutsideLightsPin, new Behavior(kOutsideActivity,'>',0, 100));
  // iot->addBehavior(kMainLightsPin, new Behavior(kOverheadActivity,'>',0, 100));
  // iot->addBehavior(kCounterLightsPin, new Behavior(kKitchenActivity,'>',0, 5));
}

/************/
/*** LOOP ***/
/************/
void loop() {
  iot->loop();
}
