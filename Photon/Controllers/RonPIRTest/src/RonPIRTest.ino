/**
RonPIRTest Controller
 
Description: This sketch is for testing a PIR device connected to D0
Refer to www.ElectronicWings.com/particle/pir-sensor-interfacing-with-particle-photon
 
Author: Ron Lisle

Hardware
 - built-in blue LED     D7
 - PIR                   A5
 
To update controller: particle flash RonPIRTest
 
**/

#include <IoT.h>
#include <PatriotPIR.h>

#define PIR_PIN A5

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

IPAddress myAddress(192,168,50,40);
IPAddress netmask(255,255,255,0);
IPAddress gateway(192,168,50,1);
IPAddress dns(192,168,50,1);

void setup() {

    WiFi.selectAntenna(ANT_INTERNAL);
    setWifiStaticIP();
    IoT::begin("192.168.50.33", "RonTest");

    Device::add(new PIR(PIR_PIN, "Movement", "Office", 500));

    RGB.control(true);
    RGB.color(255,0,0);     // Red
}

void setWifiStaticIP() {
    WiFi.setStaticIP(myAddress, netmask, gateway, dns);
    WiFi.useStaticIP();
}

void loop() {

    IoT::loop();

    int motionChanged = Device::getChangedValue("Movement");
    if( motionChanged != -1 ) {
        Log.info("Movement changed %d",motionChanged);
        if( motionChanged == 0 ) {
            RGB.color(0,255,0);
        } else {
            RGB.color(64,0,0);
        }
    }
}
