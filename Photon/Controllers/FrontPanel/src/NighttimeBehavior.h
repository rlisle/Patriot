//------------
// Nighttime Behaviors
//------------

void nighttime();
void daytime();

void handleNighttime(int value, int oldValue) {

    Log.info("handleNighttime");

    if(value > 0 && oldValue == 0) {
        nighttime();
    }
    if(value == 0 && oldValue > 0) {
        daytime();
    }
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

void nighttime() {
    // Turn on some lights
    setAllOutsideLights(100);
    setAllInsideLights(33);
}

void daytime() {
    // Turn off lights
    setAllOutsideLights(0);
    setAllInsideLights(0);
}
