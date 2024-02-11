//------------
// Sleeping Behaviors
//------------

#include <IoT.h>

void sleeping();
void awake();

void handleSleeping(int value, int oldValue) {
    Log.info("handleSleeping handler");

    if(value > 0 && oldValue == 0) {
        sleeping();
    }
    if(value == 0 && oldValue > 0) {
        awake();
    }
}

void sleeping() {
    Log.info("is Sleeping");
    // Turn off everything
    Device::setValue("Light", 0);
}

void awake() {
    Log.info("is Awake");
    // Turn on desk (HomeKit)
}
