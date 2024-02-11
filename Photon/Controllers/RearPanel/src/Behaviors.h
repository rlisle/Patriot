//------------
// Door Behaviors
//------------
#define OFFICE_DOOR_LIGHT_TIMEOUT 15*1000

void setAllLights(int);
void setAllInsideLights(int);
void setAllOutsideLights(int);
void setInsideNightLights();
void setInsideDayLights();

// Update devices based on new state of things
void updateLights() {
    if(nighttime && sleeping) {     // Sleeping
        setAllLights(0);
        Device::setValue("Curtain", 0);

    } else if(nighttime) {          // Nighttime
        setAllOutsideLights(100);
        setInsideNightLights();

    } else {                        // Daytime
        setAllOutsideLights(0);
        setInsideDayLights();
    }

    if(nighttime && officeDoorOpen) {
        Device::setValue("RearPorch", 100);
    }
    if(nighttime && !officeDoorOpen && isTimingOfficeDoor) {
        if(msecsLastDoorEvent < msecs() + OFFICE_DOOR_TIMEOUT_MSECS) {
            isTimingOfficeDoor = false;
            Device::setValue("RearPorch", 0);
        } else {
            Device::setValue("RearPorch", 100);
        }
    }
    //TODO: Cleaning
}

void setAllLights(int value) {
    setAllInsideLights(value);
    setAllOutsideLights(value);
}

void setAllInsideLights(int value) {
    Device::setValue("OfficeCeiling", value);
    Device::setValue("Loft", value);
    Device::setValue("Piano", value);
}

void setAllOutsideLights(int value) {
    Device::setValue("RampPorch", value);
    Device::setValue("RampAwning", value);
    Device::setValue("RearPorch", value);
    Device::setValue("RearAwning", value);
}

void setInsideNightLights() {
    Device::setValue("OfficeCeiling", 20);
    Device::setValue("Loft", 0);
    Device::setValue("Piano", 100);
}

void setInsideDayLights() {
    setAllOutsideLights(0);
    Device::setValue("OfficeCeiling", 0);
    Device::setValue("Loft", 0);
    Device::setValue("Piano", 100);
}
