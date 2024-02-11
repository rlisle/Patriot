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
    Device::setValue("LeftVertical", 0);
    Device::setValue("Couch", 0);
}

void awake() {
    // Turn on desk (HomeKit)
}
