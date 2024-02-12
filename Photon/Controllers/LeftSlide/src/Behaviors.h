//------------
// Behaviors
//------------

#define LIVINGROOM_MOTION_TIMEOUT_MSECS 15*1000

void setAllLights(int);
void setBedtimeLights();
void setSleepingLights();
void setEveningAwakeLights();
void setPreDawnAwakeLights();
void setDaytimeLights();

bool isAM() {
    return Time.hour() <= 12;
}

// Use sleeping, nighttime, and isAM() to determine PartOfDay
PartOfDay partOfDay() {
    if(sleeping) {
        return Sleeping;
    }
    if(nighttime) {
        if(isAM()) {
            return AwakeEarly;
        } else {
            return Evening;
        }
    }
    if(isAM()) {
        return Morning;
    }
    return Afternoon;
}

void updateLights() {
    switch(partOfDay()) {
        case Sleeping:
            Device::setValue("Curtain", 0);
            setSleepingLights();
            break;
        case AwakeEarly:
            setPreDawnAwakeLights();
            break;
        case Morning:
        case Afternoon:
            setDaytimeLights();
            break;
        case Evening:
            setEveningAwakeLights();
            break;
        case Bedtime:
            setBedtimeLights();
            break;
    }
    //TODO: Cleaning
}

void setAllLights(int value) {
    Device::setValue("LeftVertical", value);
    Device::setValue("Couch", value);
}

void setSleepingLights() {
    setAllLights(0);
}

void setEveningAwakeLights() {
    // Nothing to do
}

void setBedtimeLights() {
    setAllLights(0);
}

void setPreDawnAwakeLights() {
    // Nothing to do
}

void setDaytimeLights() {
    // Nothing to do
}
