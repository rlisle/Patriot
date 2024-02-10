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
    // Turn on some lights
    Device::setValue("LeftVertical", 20);
    Device::setValue("Couch", 20);
}

void daytime() {
    // Turn off lights
    Device::setValue("LeftVertical", 0);
    Device::setValue("Couch", 0);
}
