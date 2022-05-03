/*******************************************************
Patriot PIR Example

This example shows how to use the Patriot PIR plugin
It includes:

When a PIR is turned on, the command <name>:100 will be
published.
When the switch is turned off, it will publish
the command <name>:0.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2021-12-18: Initial creation by modifying Switch plugin.
********************************************************/

#include <IoT.h>
#include <PatriotPIR.h>
#include <PatriotLight.h>

#define LIVINGROOM_MOTION_TIMEOUT 2*60*1000

void setup() {

    // MQTT Broker IP Address, and name of your Photon
    IoT::begin("192.168.?.??","MyPhoton");

    // Create PIR sensor
    Device::add(new PIR(A0, "LivingRoomMotion", "Living Room", LIVINGROOM_MOTION_TIMEOUT));
    
    // Add a device to control (built-in blue LED)
    Device::add(new Light(D7, "BlueLED", "Living Room", 0));

}

void loop() {
    IoT::loop();

    int livingRoomMotionChanged = Device::getChangedValue("LivingRoomMotion");

    // Motion?
    if(livingRoomMotionChanged > 0 ) {
        Log.info("LivingRoom motion detected");
        Device::setValue("BlueLED", 100);
    } else if(livingRoomMotionChanged == 0){
        Log.info("LivingRoom motion stopped");
        Device::setValue("BlueLED", 0);
    }
}
