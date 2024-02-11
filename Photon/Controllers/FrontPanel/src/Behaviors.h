//------------
// Behaviors
//------------
void setAllLights(int);
void setAllInsideLights(int);
void setAllOutsideLights(int);
void setInsideNightLights();
void setInsideDayLights();

// Set Lights
// Update devices based on new state of things
void updateLights() {
    //TODO: Cleaning

    if(nighttime && sleeping) {     // Sleeping
        setAllLights(0);

    } else if(nighttime) {          // Nighttime
        setAllOutsideLights(100);
        setInsideNightLights();

    } else {                        // Daytime
        setAllOutsideLights(0);
        setInsideDayLights();
    }
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

void setInsideNightLights() {
    // Turn on some lights
    setAllOutsideLights(100);
    Device::setValue("KitchenCeiling", 0);
    Device::setValue("Sink", 33);
    Device::setValue("LeftTrim", 100);
    Device::setValue("RightTrim", 100);
    Device::setValue("Ceiling", 25);
    Device::setValue("Cabinets", 25);
}

void setInsideDayLights() {
    setAllOutsideLights(0);
    Device::setValue("KitchenCeiling", 0);
    Device::setValue("Sink", 50);
    Device::setValue("LeftTrim", 100);
    Device::setValue("RightTrim", 100);
    Device::setValue("Ceiling", 0);
    Device::setValue("Cabinets", 0);
}
