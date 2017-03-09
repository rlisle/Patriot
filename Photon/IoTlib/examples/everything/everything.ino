/*******************************************************
Everything ParticleIoT Example

This example contains one of each IoT device type
supported by ParticleIoT. This include the following:

 * 4 LED Lights (Red, Green)
 * 4 Switches   (left, right)
 * DHT11 Temperature & Humidity sensor
 * HC-SR04 Ultrasonic Proximity Sensor
 * Photoresistor light level sensor
 * Fan              (TODO:)
 * Servo Motor      (TODO:)
 * Stepper Motor    (TODO:)

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
#define kLight1         "light1"    // Can be any name you want
#define kLight2         "light2"    // "
#define kLight3         "light3"    // "
#define kLight4         "light4"    // "

// Switch inputs
#define kSwitch1Pin     D4      // pin 17
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
#define kEvent3         "cook"      // Turn on kitchen lights fully
#define kEvent4         "bedtime"   // Turn on bedroom lights, all others off

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

    iot->addLight(kLed1Pin, kLight1);
    iot->addLight(kLed2Pin, kLight2);
    iot->addLight(kLed3Pin, kLight3);
    iot->addLight(kLed4Pin, kLight4);

    iot->addSwitch(kSwitch4Pin, kEvent4);
    iot->addSwitch(kSwitch3Pin, kEvent3);
    iot->addSwitch(kSwitch2Pin, kEvent2);
    iot->addSwitch(kSwitch1Pin, kEvent1);

    iot->monitorPresence(kTriggerPin, kEchoPin, 3, 36, kEvent3);
    iot->monitorTemperature(kDhtPin, kDhtType, kEvent2, 5000);

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
