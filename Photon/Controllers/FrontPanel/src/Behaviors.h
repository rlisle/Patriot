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

// Update all devices managed by this Photon2
void updateLights() {
    PartOfDay pod = partOfDay();

    // When cleaning is set, all inside lights are turned on
    // Turn on all outside lights also if it is nighttime
    // Assuming that not bedtime or sleeping when cleaning
    if(is("Cleaning")) {
        // Turn off other statuses
        set("Bedtime", 0);
        set("Sleeping", 0);

        // Set lights
        set("KitchenCeiling", 100);
        set("Cabinets", 100);
        set("SinkLamp", 100);
        set("LeftTrim", 100);
        set("RightTrim", 100);
        set("Ceiling", 100);
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
            set("KitchenCeiling", 0);
            set("Cabinets", 0);
            set("SinkLamp", 50);
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
            set("KitchenCeiling", 0);
            set("Cabinets", 0);
            set("SinkLamp", 0);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;
        case AwakeEarly:
            set("SinkLamp", 40);
            //TODO: turn on desk lamps
            break;
        case Morning:
        case Afternoon:
            set("KitchenCeiling", 0);
            set("Cabinets", 0);
            set("SinkLamp", 100);
            set("LeftTrim", 0);
            set("RightTrim", 0);
            set("Ceiling", 0);
            set("DoorSide", 0);
            set("OtherSide", 0);
            set("FrontAwning", 0);
            set("FrontPorch", 0);
            break;
        case Evening:
            set("KitchenCeiling", 20);
            // Don't set cabinets - they reflect off TV
            //set("Cabinets", 0);
            set("SinkLamp", 50);
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

void setSinkOrOverride(int percent) {
    if(is("Sink")) {
        set("SinkLamp", value("Sink"));
    } else {
        set("SinkLamp", percent);
    }
}
