/*******************************************************
Everything ParticleIoT Example

This example contains one or more of each IoT device type
supported by ParticleIoT. This includes the following:

 * 4 LED Lights
     - Red   = garage
     - Green = kitchen
     - Blue  = living room
     - Amber = bedroom
 * 4 Switches
 * DHT11 or DHT22 Temperature & Humidity sensor
 * HC-SR04 Ultrasonic Proximity Sensor
 * Photoresistor light level sensor (TODO:)
 * Fan                              (TODO:)
 * Servo Motor                      (TODO:)
 * Stepper Motor                    (TODO:)


http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-11: TODO: add remaining devices.
2017-03-07: Initial creation
********************************************************/

/****************/
/*** INCLUDES ***/
/****************/
#include <IoT.h>

/*****************/
/*** CONSTANTS ***/
/*****************/
// DHT11 or DHT22 Humidity & Temperature Sensor
#define kDhtType        DHT22   // can be DHT11 or DHT22
#define kDhtPin         A2      // pin 10

// HD-SR04 Ultrasonic Proximity Sensor
#define kTriggerPin     A1      // pin 11
#define kEchoPin        A0      // pin 12

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

// Switch inputs
#define kSwitch1Pin     D4      // pin 17 Can be any event you want
#define kSwitch2Pin     D5      // pin 18
#define kSwitch3Pin     D6      // pin 19
#define kSwitch4Pin     D7      // pin 20

// Photoresistor ambient light level sensor
#define kPhotoPin       A4      // pin 8

// Fan
// TBD DAC (3v)?

// Servo Motor
//TBD A5 (3v)?

// Stepper motor
#define kStepperPin1    D0      // pin 13
#define kStepperPin2    D1      // pin 14
#define kStepperPin3    D2      // pin 15
#define kStepperPin4    D3      // pin 16

// Event names can be anything you want, but should indicate activity
// and not just the name of an appliance. For example, "sleep"
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

    iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, kEvent3);
    iot->monitorTemperature(kDhtPin, kDhtType, kEvent5, 5000);

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
