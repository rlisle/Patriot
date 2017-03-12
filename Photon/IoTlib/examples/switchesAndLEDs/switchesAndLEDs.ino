/*******************************************************
Switches and LEDs ParticleIoT Example

This example contains 4 switches and 4 LEDs:

 * 4 LED Lights
     - Red   = garage       A3      Photon pin 9
     - Green = kitchen      RX      Photon pin 4
     - Blue  = living room  TX      Photon pin 3
     - Amber = bedroom      WKP     Photon pin 5

 * 4 Switches
     - Switch 1             D4      Photon pin 17
     - Switch 2             D5      Photon pin 18
     - Switch 3             D6      Photon pin 19
     - Switch 4             D7      Photon pin 20

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-11: Created switch & leds
2017-03-07: Initial creation
********************************************************/

/****************/
/*** INCLUDES ***/
/****************/
#include <IoT.h>

/*****************/
/*** CONSTANTS ***/
/*****************/
// LED pins
#define kLed1Pin        A3      // pin 9
#define kLed2Pin        RX      // pin 4
#define kLed3Pin        TX      // pin 3
#define kLed4Pin        WKP     // pin 5

// LED light names
#define kLight1         "garage"        // Can be any name you want
#define kLight2         "kitchen"       // "
#define kLight3         "livingroom"    // "
#define kLight4         "bedroom"       // "

// Switch pins
#define kSwitch1Pin     D4      // pin 17 Can be any event you want
#define kSwitch2Pin     D5      // pin 18
#define kSwitch3Pin     D6      // pin 19
#define kSwitch4Pin     D7      // pin 20

// Event names can be anything you want, but should indicate activity
// and not just the name of an appliance.
#define kEvent1         "sleep"     // Turn off all lights except night lights
#define kEvent2         "watchtv"   // Dim living room lights
#define kEvent3         "cook"      // Turn on kitchen lights, fan
#define kEvent4         "bedtime"   // Turn on bedroom lights, all others off
#define kEvent5         "kitchen"   // Kitchen temp & humidity

/*****************/
/*** VARIABLES ***/
/*****************/
IoT *iot;

/*************/
/*** SETUP ***/
/*************/
void setup() {
    initializeIoT();
    addLights();
    addSwitches();

    iot->exposeControllers();
    iot->exposeActivities();
}

void initializeIoT() {
    iot = IoT::getInstance();
    iot->setControllerName("lislerv");
    iot->setPublishName("RonTest");
    iot->begin();
}

void addSwitches() {
    iot->addSwitch(kSwitch4Pin, kEvent4);
    iot->addSwitch(kSwitch3Pin, kEvent3);
    iot->addSwitch(kSwitch2Pin, kEvent2);
    iot->addSwitch(kSwitch1Pin, kEvent1);
}

void addLights() {
    iot->addLight(kLed1Pin, kLight1);
    iot->addLight(kLed2Pin, kLight2);
    iot->addLight(kLed3Pin, kLight3);
    iot->addLight(kLed4Pin, kLight4);
}

/************/
/*** LOOP ***/
/************/
void loop() {
    iot->loop(millis());
}
