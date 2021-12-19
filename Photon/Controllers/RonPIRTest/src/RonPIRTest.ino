/**
RonPIRTest Controller
 
Description: This sketch is for testing a PIR device connected to D0
Refer to www.ElectronicWings.com/particle/pir-sensor-interfacing-with-particle-photon
 
Author: Ron Lisle

Hardware
 - built-in blue LED     D7
 - PIR                   D0
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

    int sensor_output;
    sensor_output = digitalRead(PIR_PIN);
    if(warm_up == 1) {
        Serial.println("Warming Up");
        warm_up = 0;
        RGB.color(255,255,0);
        delay(2000);
        
    } else if(sensor_output != current_state) {
        current_state = sensor_output;
        if(sensor_output == LOW) {
            Serial.println("No object in sight");
            RGB.color(255,0,0);
        } else {
            Serial.println("Objected Detected");
            RGB.color(0,255,0);
        }
    }
}
