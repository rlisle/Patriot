/******************************************************************
Example starting point sketch for ParticleIoT library

This example contains no devices.
It is provided for comparison with other example sketches.

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-11: Initial creation
******************************************************************/

/****************/
/*** INCLUDES ***/
/****************/
#include <IoT.h>

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
}

/************/
/*** LOOP ***/
/************/
void loop() {
    long currentMillis = millis();
    iot->loop(currentMillis);
}
