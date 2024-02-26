//------------
// Behaviors
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   Kitchen
//   LivingRoomMotion
//   Nighttime
//   Outside
//   RonHome
//   ShelleyHome
//   Sink
//   Sleeping
//------------
#define FRONT_DOOR_LIGHT_TIMEOUT 15*1000

void setSink(int percent) {
    if(is("Sink")) {
        Log.info("FP set sinkLamp override: %d", value("Sink"));
        set("SinkLamp", value("Sink"));
    } else {
        Log.info("FP set sinkLamp percent: %d", percent);
        set("SinkLamp", percent);
    }
}

void setKitchenCeiling(int percent) {
    if(is("Kitchen")) {
        set("KitchenCeiling", value("Kitchen"));
    } else {
        set("KitchenCeiling", percent);
    }
}

void setCabinets(int percent) {
    if(is("Kitchen")) {
        set("Cabinets", value("Kitchen"));
    } else {
        set("Cabinets", percent);
    }
}

// Update all devices managed by this Photon2
void updateLights() {
    Log.info("FP updateLights");

    PartOfDay pod = partOfDay();

    // When cleaning is set, all inside lights are turned on
    // Turn on all outside lights also if it is nighttime
    // Assuming that not bedtime or sleeping when cleaning
    if(is("Cleaning")) {
        // Turn off other statuses
        set("Bedtime", 0);
        set("Sleeping", 0);

        // Set inside lights
        setKitchenCeiling(100);
        setCabinets(100);
        setSink(100);
        set("LeftTrim", 100);
        set("RightTrim", 100);
        set("Ceiling", 100);

        // Set outside lights TODO: update when door implemented
        if(pod == Evening) {
            set("DoorSide", 100);
            set("OtherSide", 100);
            set("FrontAwning", 100);
            set("FrontPorch", 100);
        }
        return;
    }

    switch(pod) {
        case Retiring:
            // Turn off other statuses
            set("Cleaning", 0);
            set("Kitchen", 0);
            set("Outside", 0);
            set("Sink", 0);
            set("Sleeping", 0);

            // Set lights
            setKitchenCeiling(0);
            setCabinets(0);
            setSink(5);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;

        case Asleep:                  // Don't assume bedtime was set
            // Turn off other statuses
            set("Bedtime", 0);
            set("Cleaning", 0);
            set("Kitchen", 0);
            set("Outside", 0);
            set("Sink", 0);

            // Set lights
            setKitchenCeiling(0);
            setCabinets(0);
            setSink(0);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;
        case AwakeEarly:
            setSink(5);
            //TODO: turn on desk lamps
            break;
        case Morning:
        case Afternoon:
            setKitchenCeiling(0);
            setCabinets(0);
            setSink(0);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;
        case Evening:
            setKitchenCeiling(20);
            // Don't set cabinets unless overridden - they reflect off TV
            setCabinets(0);
            setSink(5);
            set("LeftTrim", 100);
            set("RightTrim", 100);
            set("Ceiling", 20);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 100);
            break;
    }
}

