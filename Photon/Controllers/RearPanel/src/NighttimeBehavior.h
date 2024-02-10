//------------
// Nighttime Behaviors
//------------

#include <IoT.h>

void nighttime();
void daytime();

void handleNighttime(int value, int oldValue) {

    Log.info("handleNighttime");

    if(value > 0 && oldValue == 0) {
        nighttime();
    }
    if(value == 0 && oldValue > 0) {
        daytime();
    }
}

void nighttime() {
    // Close curtains
    Device::setValue("Curtain", 0);
    // Turn on some lights
    //Device::setValue("OfficeCeiling", 100);
    //Device::setValue("Loft", 100);
    //Device::setValue("RampPorch", 100);
    //Device::setValue("RampAwning", 100);
    Device::setValue("RearPorch", 100);
    //Device::setValue("RearAwning", 100);
    Device::setValue("Piano", 100);
}

void daytime() {
    // Turn off lights
    //Device::setValue("OfficeCeiling", 100);
    //Device::setValue("Loft", 100);
    Device::setValue("RampPorch", 0);
    Device::setValue("RampAwning", 0);
    Device::setValue("RearPorch", 0);
    Device::setValue("RearAwning", 0);
    Device::setValue("Piano", 0);
}
