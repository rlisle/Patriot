//------------
// Door Behaviors
//------------
#define OFFICE_DOOR_LIGHT_TIMEOUT 15*1000

void setAllLights(int);
void setAllInsideLights(int);
void setAllOutsideLights(int);
void setInsideNightLights();
void setInsideDayLights();

bool isAM() {
    return Time.hour() <= 12;
}

// Use sleeping, nighttime, and isAM() to determine PartOfDay
PartOfDay partOfDay() {
    if(sleeping) {
        return .Sleeping;
    }
    if(nighttime) {
        if(isAM()) {
            return .AwakeEarly
        } else {
            return .Evening
        }
    }
    if(isAM()) {
        return .Morning
    }
    return .Afternoon
}

// Update devices based on new state of things
void updateLights() {
    switch(partOfDay()) {
        case .Sleeping:
            Device::setValue("Curtain", 0);
            setSleepingLights();
            break;
        case .AwakeEarly:
            setPreDawnAwakeLights();
            break;
        case .Morning:
        case .Afternoon:
            setDaytimeLights();
            break;
        case .Evening:
            setEveningAwakeLights();
            break;
        case .Bedtime:
            setBedtimeLights();
            break;
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

void setSleepingLights() {
    Device::setValue("Curtain", 0);
    setAllLights(0);
    if(officeDoorOpen || isTimingOfficeDoor) {        
        Device::setValue("RearPorch", 100);
    }
}

void setEveningAwakeLights() {
    setAllOutsideLights(100);
    Device::setValue("OfficeCeiling", 20);
    Device::setValue("Loft", 0);
    Device::setValue("Piano", 100);
}

void setPreDawnAwakeLights() {
    setAllOutsideLights(0);
    Device::setValue("OfficeCeiling", 20);
    Device::setValue("Loft", 0);
    Device::setValue("Piano", 100);
}

void setDaytimeLights() {
    setAllOutsideLights(0);
    Device::setValue("OfficeCeiling", 0);
    Device::setValue("Loft", 0);
    Device::setValue("Piano", 100);
}
