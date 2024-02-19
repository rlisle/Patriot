//------------
// Behaviors
//------------
#define FRONT_DOOR_LIGHT_TIMEOUT 15*1000

void setAllLights(int);
void setAllInsideLights(int);
void setAllOutsideLights(int);
void setBedtimeLights();
void setSleepingLights();
void setEveningAwakeLights();
void setPreDawnAwakeLights();
void setDaytimeLights();

//TODO: Move to IoT
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

//TODO: Move to IoT
// Use sleeping, nighttime, and isAM() to determine PartOfDay
PartOfDay partOfDay() {
    if(is("sleeping")) {
        return Sleeping;
    }
    if(is("bedtime")) {
        return Bedtime;
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
    Log.info("FP updateLights");
    if(is("cleaning")) {
        Log.info("FP updateLights cleaning");
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
    setAllInsideLights(value);
    setAllOutsideLights(value);
}

void setAllInsideLights(int value) {
    set("KitchenCeiling", value);
    set("Sink", value);
    set("LeftTrim", value);
    set("RightTrim", value);
    set("Ceiling", value);
    set("Cabinets", value);
}

void setAllOutsideLights(int value) {
    set("DoorSide", value);
    set("OtherSide", value);
    set("FrontAwning", value);
    set("FrontPorch", value);
}

void setSleepingLights() {
    setAllLights(0);
    // if(is("FrontDoor") || isTimingLivingRoomDoor) {        
    //     set("FrontPorch", 100);
    // }
}

void setEveningAwakeLights() {
    setAllOutsideLights(100);
    set("KitchenCeiling", 50);
    set("Sink", 5);
    set("LeftTrim", 50);
    set("RightTrim", 10);
    set("Ceiling", 80);
    set("Cabinets", 40);
}

void setBedtimeLights() {
    setAllOutsideLights(0);
    set("KitchenCeiling", 0);
    set("Sink", 5);
    set("LeftTrim", 0);
    set("RightTrim", 0);
    set("Ceiling", 0);
    set("Cabinets", 0);
}

void setPreDawnAwakeLights() {
    setAllOutsideLights(0);
    setAllInsideLights(0);
    set("Sink", 5);
}

void setDaytimeLights() {
    setAllOutsideLights(0);
    set("KitchenCeiling", 0);
    set("Sink", 20);
    set("LeftTrim", 0);
    set("RightTrim", 0);
    set("Ceiling", 0);
    set("Cabinets", 0);
}
