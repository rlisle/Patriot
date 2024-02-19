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

// Shortcuts
bool is(String name) {
    return Device::get("name")->value() > 0;
}

int value(String name) {
    return Device::get("name")->value();
}

int set(String name, int value) {
    return Device::setValue(name,value);
}

bool isAM() {
    return Time.hour() <= 12;
}

// Use sleeping, nighttime, and isAM() to determine PartOfDay
PartOfDay partOfDay() {
    if(sleeping) {
        return Sleeping;
    }
    if(is("bedtime")) {
        return Bedtime;
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
    if(is("cleaning")) {
        Log.info("LS updateLights cleaning");
        setAllInsideLights(100);
        return;                     // Assumes daytime, so no need to continue
    }
    switch(partOfDay()) {
        case Sleeping:
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
