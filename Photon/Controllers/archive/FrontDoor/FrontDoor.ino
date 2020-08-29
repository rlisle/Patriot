/*
 * Front Door Controller
 *
 * This sketch controls the main, cabinet, and outside lights.
 * It handles 4 wall switches located by the front door.
 * Optionally, it will support both inside and outside motion detectors,
 * inside and outside temperature & humidity, a door bell switch,
 * and inside and outside light level sensors.
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoT
 * 3. IMU accelerometer, gyroscope, magnetometer
 *    - SDA Serial Data In  (D0)
 *    - SCL Serial Clock    (D1)
 * 4. 4 wall switches       Stepper1 (D4, D5, D6, D7)
 * 5. 3 x LED driver boards:
 *    - Main lights         LED1 (WKP)
 *    - Outside lights      LED2 (A5)
 *    - Counter lights      LED3 (A4)
 * 6. 2 x Motion detectors  (D2, D3)
 * 7. 2 x Light Sensor      (A0, A1)
 * 8. Door bell switch      (A2)
 * 9. 2 x DHT11             (Tx, Rx)
 * Available for future use: A3, DAC (neither 5v tolerant)
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
  iot->start("FrontDoor");

  setupLights();
  setupSwitches();
  setupBehaviors();

//  iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, "couch left");
}

void setupLights() {
  iot->addLight(kOutsideLightsPin, kOutsideLights);
  iot->addLight(kMainLightsPin, kMainLights);
  iot->addLight(kCounterLightsPin, kCounterSpots);
}

void setupSwitches() {
  iot->addSwitch(kSwitch4Pin,kKitchenActivity); // Right-most switch
  iot->addSwitch(kSwitch3Pin,kCoffeeActivity);
  iot->addSwitch(kSwitch2Pin,kOverheadActivity);
  iot->addSwitch(kSwitch1Pin,kOutsideActivity);    // Left-most switch
}

void setupBehaviors() {
  iot->addBehavior(kOutsideLights, new Behavior(kOutsideActivity,'>',0, 100));
  iot->addBehavior(kMainLights, new Behavior(kOverheadActivity,'>',0, 100));
  iot->addBehavior(kCounterSpots, new Behavior(kKitchenActivity,'>',0, 5));
}

/************/
/*** LOOP ***/
/************/
void loop() {
  iot->loop();
}
