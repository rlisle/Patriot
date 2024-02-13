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
    setAllInsideLights(value);
    setAllOutsideLights(value);
}

void setAllInsideLights(int value) {
    Device::setValue("KitchenCeiling", value);
    Device::setValue("Sink", value);
    Device::setValue("LeftTrim", value);
    Device::setValue("RightTrim", value);
    Device::setValue("Ceiling", value);
    Device::setValue("Cabinets", value);
}

void setAllOutsideLights(int value) {
    Device::setValue("DoorSide", value);
    Device::setValue("OtherSide", value);
    Device::setValue("FrontAwning", value);
    Device::setValue("FrontPorch", value);
}

// void setInsideNightLights() {
//     // Turn on some lights
//     setAllOutsideLights(100);
//     Device::setValue("KitchenCeiling", 0);
//     Device::setValue("Sink", 33);
//     Device::setValue("LeftTrim", 100);
//     Device::setValue("RightTrim", 100);
//     Device::setValue("Ceiling", 25);
//     Device::setValue("Cabinets", 25);
// }

// void setInsideDayLights() {
//     setAllOutsideLights(0);
//     Device::setValue("KitchenCeiling", 0);
//     Device::setValue("Sink", 50);
//     Device::setValue("LeftTrim", 100);
//     Device::setValue("RightTrim", 100);
//     Device::setValue("Ceiling", 0);
//     Device::setValue("Cabinets", 0);
// }

void setSleepingLights() {
    setAllLights(0);
    if(livingRoomDoorOpen || isTimingLivingRoomDoor) {        
        Device::setValue("FrontPorch", 100);
    }
}

void setEveningAwakeLights() {
    setAllOutsideLights(100);
    Device::setValue("KitchenCeiling", 50);
    Device::setValue("Sink", 5);
    Device::setValue("LeftTrim", 50);
    Device::setValue("RightTrim", 10);
    Device::setValue("Ceiling", 80);
    Device::setValue("Cabinets", 40);
}

void setBedtimeLights() {
    setAllOutsideLights(0);
    Device::setValue("KitchenCeiling", 0);
    Device::setValue("Sink", 5);
    Device::setValue("LeftTrim", 0);
    Device::setValue("RightTrim", 0);
    Device::setValue("Ceiling", 0);
    Device::setValue("Cabinets", 0);
}

void setPreDawnAwakeLights() {
    setAllOutsideLights(0);
    setAllInsideLights(0);
    Device::setValue("Sink", 5);
}

void setDaytimeLights() {
    setAllOutsideLights(0);
    Device::setValue("KitchenCeiling", 0);
    Device::setValue("Sink", 20);
    Device::setValue("LeftTrim", 0);
    Device::setValue("RightTrim", 0);
    Device::setValue("Ceiling", 0);
    Device::setValue("Cabinets", 0);
}
