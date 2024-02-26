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

void setSinkOrOverride(int percent) {
    if(is("Sink")) {
        Log.info("FP set sinkLamp override: %d", value("Sink"));
        set("SinkLamp", value("Sink"));
    } else {
        Log.info("FP set sinkLamp percent: %d", percent);
        set("SinkLamp", percent);
    }
}

void setKitchenCeilingOrOverride(int percent) {
    if(is("Kitchen")) {
        set("KitchenCeiling", value("Kitchen"));
    } else {
        set("KitchenCeiling", percent);
    }
}

void setCabinetsOrOverride(int percent) {
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
        setKitchenCeilingOrOverride(100);
        setCabinetsOrOverride(100);
        setSinkOrOverride(100);
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
            setKitchenCeilingOrOverride(0);
            setCabinetsOrOverride(0);
            setSinkOrOverride(50);
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
            setKitchenCeilingOrOverride(0);
            setCabinetsOrOverride(0);
            setSinkOrOverride(0);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;
        case AwakeEarly:
            setSinkOrOverride(10);
            //TODO: turn on desk lamps
            break;
        case Morning:
        case Afternoon:
            setKitchenCeilingOrOverride(0);
            setCabinetsOrOverride(0);
            setSinkOrOverride(100);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;
        case Evening:
            setKitchenCeilingOrOverride(20);
            // Don't set cabinets unless overridden - they reflect off TV
            setCabinetsOrOverride(0);
            setSinkOrOverride(40);
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

