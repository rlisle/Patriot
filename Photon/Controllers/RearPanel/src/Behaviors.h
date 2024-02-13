//------------
// Door Behaviors
//------------
#define OFFICE_DOOR_LIGHT_TIMEOUT 15*1000

void setAllLights(int);
void setAllInsideLights(int);
void setAllOutsideLights(int);
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
    Log.info("RP updateLights");
    if(cleaning) {                  //TODO: move to IoT
        Log.info("RP updateLights cleaning");
        setAllInsideLights(100);
        return;                     // Assumes daytime, so no need to continue
    }
    switch(partOfDay()) {
        case Sleeping:
            Log.info("RP updateLights Sleeping");
            setSleepingLights();
            break;
        case AwakeEarly:
            Log.info("RP updateLights AwakeEarly");
            setPreDawnAwakeLights();
            break;
        case Morning:
        case Afternoon:
            Log.info("RP updateLights daytime");
            setDaytimeLights();
            break;
        case Evening:
            Log.info("RP updateLights evening");
            setEveningAwakeLights();
            break;
        case Bedtime:
            Log.info("RP updateLights bedtime");
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
    Device::setValue("OfficeCeiling", 5);
    Device::setValue("Loft", 0);
    Device::setValue("Piano", 100);
}

void setBedtimeLights() {  // Same as above but turn off outside
    setAllOutsideLights(0);
    Device::setValue("OfficeCeiling", 5);
    Device::setValue("Loft", 0);
    Device::setValue("Piano", 5);
    if(officeDoorOpen || isTimingOfficeDoor) {        
        Device::setValue("RearPorch", 100);
    }
}

void setPreDawnAwakeLights() {
    setAllOutsideLights(0);
    Device::setValue("OfficeCeiling", 5);
    Device::setValue("Loft", 0);
    Device::setValue("Piano", 5);
    if(officeDoorOpen || isTimingOfficeDoor) {        
        Log.info("RP PreDawnAwakeLights door open");
        Device::setValue("RearPorch", 100);
    }
}

void setDaytimeLights() {
    setAllOutsideLights(0);
    Device::setValue("OfficeCeiling", 0);
    Device::setValue("Loft", 0);
    Device::setValue("Piano", 100);
}
