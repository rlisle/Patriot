/*
 * Template Controller
 *
 * This sketch provides a template for creating controllers.
 * 1. Read switches and publish changes.
 * 2. Read DHT11 temp sensor and publish changes
 * 3. Control a fan based on the read temperature
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoTv2
 * 3. LED lights (Up to 4 total lights and/or motors)
 * 4. Fan motors (Up to 4 total lights and/or motors)
 * 5. Switches (Up to 18: can use any pins)
 * 6. PIR Motion Sensor (Up to 3: use 5v pin)
 * 7. Ultrasonic proximity detector (Up to 3: use 5v pin)
 * 8. Light level sensor (dedicated kPhotoPin, but can use other pins)
 * 9. DHT11 temp/humidity sensor (can use any 3v or 5v pin)
 *    Left to right: +5/3.3, Data In, n/c, Gnd
 *
 * History
 *  8/29/16 Create template based on sections from existing controllers.
 */

/****************/
/*** INCLUDES ***/
/****************/
#include "iot.h"

/*****************/
/*** CONSTANTS ***/
/*****************/
// These are example assignments. Use any combination as needed.
// define switch pins (Up to 18: can use any I/O pins)
#define kSwitchPin      D0

// Define light pins (Up to 4)
// Use this to map up to 4 led pins to a meaningful name
#define kLampPin        D1

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
  iot->start("<name>");

  setupLights();
  setupBehaviors();
  setupSwitches();
  iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, "<location>");
  iot->monitorTemperature(kDhtPin, kDhtType, "<location>", 5000);
}

void setupSwitches() {
  iot->addSwitch(kSwitchPin,"<event>");
}

void setupLights() {
  iot->addLight(kLightPin, "<device>");
}

void setupBehaviors() {
  iot->addBehavior("<device>", new Behavior("<activity>",'>',0, 100));
}

/************/
/*** LOOP ***/
/************/
void loop() {
  iot->loop();
}
