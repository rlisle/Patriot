/** Back Door
 *
 * This sketch controls the Photon mounted next to the back door.
 * It manages
 *    - back door switches
 *    - Outside ramp light
 *    - Rear overhead light
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoTv2
 * 3. 4 x wall switch A0, A1, A2, A5
 * 4. Overhead lights RX
 * 5. Ramp lights TX
 */

/****************/
/*** INCLUDES ***/
/****************/
#include "application.h"
#include "iot.h"

/*****************/
/*** CONSTANTS ***/
/*****************/
#define kSwitch1Pin      kStepper1Pin1
#define kSwitch2Pin      kStepper1Pin2
#define kSwitch3Pin      kStepper1Pin3
#define kSwitch4Pin      kStepper1Pin4

#define kLightPin        kPhotoPin

#define kRampLightsPin   kLed2Pin
#define kGarageLightsPin kLed3Pin

/*****************/
/*** VARIABLES ***/
/*****************/
IoT*       iot;

/*************/
/*** SETUP ***/
/*************/
void setup() {

  iot = IoT::getInstance();
  iot->start("BackDoor");

  setupLights();
  setupSwitches();
  setupBehaviors();

//  iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, "couch left");
}

void setupLights() {
  rampLights = lights.addLight(kRampLightsPin, "ramp lights");
  garageLights = lights.addLight(kGarageLightsPin, "garage lights");
}

void setupBehaviors() {
  iot->addBehavior(kRampLights, new Behavior(kRampActivity,'>',0, 100));
  iot->addBehavior(kRearOverheadLights, new Behavior(kOverheadActivity,'>',0, 100));
}

void setupSwitches() {
  iot->addSwitch(kSwitch4Pin,kOutsideActivity); // Right-most switch
  iot->addSwitch(kSwitch3Pin,kOverheadActivity);
  iot->addSwitch(kSwitch2Pin,kPianoActivity);
  iot->addSwitch(kSwitch1Pin,kComputerActivity);    // Left-most switch
}

void loop() {
  iot->loop();
}
