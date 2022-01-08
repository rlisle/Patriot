/**
RonPIRTest Controller
 
Description: This sketch is for testing a PIR device connected to D0
Refer to www.ElectronicWings.com/particle/pir-sensor-interfacing-with-particle-photon
 
Author: Ron Lisle

Hardware
 - built-in blue LED     D7
 - PIR                   D0
 
To update controller: particle flash RonPIRTest
 
**/

#include <IoT.h>
#include <PatriotPIR.h>

#define PIR_PIN D0

void setup() {
    IoT::begin("192.168.50.33", "RearPanel");

    Device::add(new PIR(PIR_PIN, "Movement", "Office"));

    RGB.control(true);
    RGB.color(255,0,0);     // Red
}

void loop() {

    IoT::loop();

    int motionChanged = Device::getChangedValue("Movement");
    if( motionChanged != -1 ) {
        Log.info("office movement changed %d",motionChanged);
        if( motionChanged == 0 ) {
            RGB.color(0,255,0);
        } else {
            RGB.color(64,0,0);
        }
    }
}
