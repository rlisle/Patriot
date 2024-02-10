//------------
// Sleeping Behaviors
//------------

#include <IoT.h>

void sleeping();
void awake();

void handleSleeping(int value, int oldValue) {
    Log.info("handleSleeping");

    if(value > 0 && oldValue == 0) {
        sleeping();
    }
    if(value == 0 && oldValue > 0) {
        awake();
    }
}

void sleeping() {
    // Turn off everything
    Device::setValue("Curtain", 0);
    Device::setValue("OfficeCeiling", 0);
    Device::setValue("Loft", 0);
    Device::setValue("RampPorch", 0);
    Device::setValue("RampAwning", 0);
    Device::setValue("RearPorch", 0);
    Device::setValue("RearAwning", 0);
    Device::setValue("Piano", 0);
}

void awake() {
    // Turn on desk (HomeKit)
}
