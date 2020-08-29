/*
 * Shower Controller
 *
 * Control the lights and fan over the shower.
 * 1. Read DHT11 temp sensor and publish changes
 * 2. Control a fan based on the read temperature
 * 3. Control lights
 * 4. Monitor proximity
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoTv2
 * 3. lights over shower
 * 4. Fan motor
 * 5. Ultrasonic proximity detector
 * 6. Light level sensor (dedicated kPhotoPin, but can use other pins)
 * 7. DHT11 temp/humidity sensor
 *    Left to right: +5/3.3, Data In, n/c, Gnd
 *
 * History
 *  12/18/16 Initial file based on Template
 */

/****************/
/*** INCLUDES ***/
/****************/
#include "iot.h"

/*****************/
/*** CONSTANTS ***/
/*****************/
#define kShowerLightPin TX
#define kShowerFanPin   RX

#define kDhtType        DHT22
#define kDhtPin         D2
#define kTriggerPin     D3
#define kEchoPin        D4

/*****************/
/*** VARIABLES ***/
/*****************/
IoT*      iot;

/*************/
/*** SETUP ***/
/*************/
void setup() {

  iot = IoT::getInstance();
  iot->monitorAlive("Shower");

  setupLights();
  setupFan();
  setupBehaviors();
  iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, "shower");
//  iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, "<location>");
  iot->monitorTemperature(kDhtPin, kDhtType, "shower", 5000);
}

void setupLights() {
  iot->addLight(kShowerLightPin, kShowerLight);
}

void setupFan() {
  iot->addFan(kShowerFan, kShowerFanPin, 0, 0, "none");
}

void setupBehaviors() {
  iot->addBehavior(kShowerLight, new Behavior(kShowerActivity,'>',0, 100));
  iot->addBehavior(kShowerLight, new Behavior(kAllLightsActivity,'>',0, 100));
  iot->addBehavior(kShowerFan, new Behavior(kShowerActivity,'>',0, 100));
}

/************/
/*** LOOP ***/
/************/
void loop() {
  iot->loop();
}
