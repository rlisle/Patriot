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
    if(is("sleeping")) {
        return Asleep;
    }
    if(is("nighttime")) {
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

// This method defines all the behavior of the system
void updateLights() {
    Log.info("RP updateLights");
    if(is("cleaning")) {
        Log.info("RP updateLights cleaning");
        setAllInsideLights(100);
        return;                     // Assumes daytime, so no need to continue
    }
    switch(partOfDay()) {
        case Asleep:
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
        case Retiring:
            Log.info("RP updateLights bedtime");
            setBedtimeLights();
            break;
    }
}

void setAllLights(int value) {
    setAllInsideLights(value);
    setAllOutsideLights(value);
}

void setAllInsideLights(int value) {
    set("OfficeCeiling", value);
    set("Loft", value);
    set("Piano", value);
}

void setAllOutsideLights(int value) {
    set("RampPorch", value);
    set("RampAwning", value);
    set("RearPorch", value);
    set("RearAwning", value);
}

void setSleepingLights() {
    set("Curtain", 0);     // Close curtain
    setAllLights(0);
    if(is("officeDoor") || isTimingOfficeDoor) {        
        set("RearPorch", 100);
    }
}

void setEveningAwakeLights() {
    setAllOutsideLights(100);
    set("OfficeCeiling", 5);
    set("Loft", 0);
    set("Piano", 100);
}

void setBedtimeLights() {  // Same as above but turn off outside
    setAllOutsideLights(0);
    set("OfficeCeiling", 5);
    set("Loft", 0);
    set("Piano", 5);
    if(is("officeDoor") || isTimingOfficeDoor) {        
        set("RearPorch", 100);
    }
}

void setPreDawnAwakeLights() {
    setAllOutsideLights(0);
    set("OfficeCeiling", 5);
    set("Loft", 0);
    set("Piano", 5);
    if(is("officeDoor") || isTimingOfficeDoor) {        
        Log.info("RP PreDawnAwakeLights door open");
        set("RearPorch", 100);
    }
}

void setDaytimeLights() {
    setAllOutsideLights(0);
    set("OfficeCeiling", 0);
    set("Loft", 0);
    set("Piano", 100);
}

// Called by livingRoomMotion
void wakeupIfAfter430am() {
    if(is("sleeping") == true 
        && Time.hour() < 10 
        && ((Time.hour() == 4 && Time.minute() >= 30)
        || (Time.hour() > 4))) {
            // Send sleeping = 0
            set("sleeping", false);
            IoT::publishValue("sleeping/set", 0);
            updateLights();
        }
}