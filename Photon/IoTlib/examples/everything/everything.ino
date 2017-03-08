/*******************************************************
Everything ParticleIoT Example

This example contains one of each IoT device type
supported by ParticleIoT. This include the following:

 * 4 LED Lights (Red, Green)
 * 4 Switches   (left, right)
 * DHT11 Temperature & Humidity sensor
 * HC-SR04 Ultrasonic Proximity Sensor
 * Photoresistor light level sensor
 * Servo Motor   (TODO:)
 * Stepper Motor (TODO:)

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-07: Initial creation
********************************************************/

#include <IoT.h>

/*****************/
/*** CONSTANTS ***/
/*****************/
// DHT11 or DHT22 Humidity & Temperature Sensor
#define kDhtType        DHT22
#define kDhtPin         A2      // pin 10

// HD-SR04 Ultrasonic Proximity Sensor
#define kTriggerPin     A1      // pin 11
#define kEchoPin        A0      // pin 12

// Display LEDs
#define kLed1Pin        A3      // pin 9
#define kLed2Pin        RX      // pin 4
#define kLed3Pin        TX      // pin 3
#define kLed4Pin        WKP     // pin 5

// Switch inputs
#define kSwitch1Pin     D4      // pin 17
#define kSwitch2Pin     D5      // pin 18
#define kSwitch3Pin     D6      // pin 19
#define kSwitch4Pin     D7      // pin 20

// Photoresistor ambient light level sensor
#define kPhotoPin       A4      // pin 8

// Servo Motor
//TBD

// Stepper motor
#define kStepperPin1    D0      // pin 13
#define kStepperPin2    D1      // pin 14
#define kStepperPin3    D2      // pin 15
#define kStepperPin4    D3      // pin 16

/*****************/
/*** VARIABLES ***/
/*****************/
IoT *iot;


/*************/
/*** SETUP ***/
/*************/
void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("lislerv");
    iot->setPublishName("RonTest");
    iot->begin();

    iot->addLight(kLed2Pin, kMicrowaveSpots);   // RX
    iot->addLight(kLed3Pin, kMicrowaveCabinet); // TX

    iot->addSwitch(kSwitch4Pin,"computer"); // Right-most switch
    iot->addSwitch(kSwitch3Pin,"booth");
    iot->addSwitch(kSwitch2Pin,"downstairs");
    iot->addSwitch(kSwitch1Pin,"outside");    // Left-most switch

    iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, "kitchen");  // A1, A0
    iot->monitorTemperature(kDhtPin, kDhtType, "kitchen", 5000);    // A2

    iot->exposeControllers();
    iot->exposeActivities();
}


/************/
/*** LOOP ***/
/************/
void loop() {
    long currentMillis = millis();
    iot->loop(currentMillis);
}
