//------------
// Behaviors
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   Couch (override)
//   LivingRoomMotion
//   Nighttime
//   RonHome
//   ShelleyHome
//   Sleeping
//------------

#define LIVINGROOM_MOTION_TIMEOUT_MSECS 15*1000

// Overrides
void setCouchCeiling(int percent) {
    if(is("Cleaning")) {
        set("CouchCeiling", 100);
    } else if(is("Couch")) {
        set("CouchCeiling", value("Couch"));
    } else {
        set("CouchCeiling", percent);
     }
}

// Update all devices managed by this Photon2
void updateLights() {
    PartOfDay pod = partOfDay();

    // When cleaning is set, all inside lights are turned on
    // Assuming that not bedtime or sleeping when cleaning
    if(is("Cleaning")) {
        // Turn off other statuses
        set("Bedtime", 0);
        set("Sleeping", 0);
        set("Couch", 0);

        // Set lights
        setCouchCeiling(100);
        set("LeftVertical", 100);
        return;
    }

    switch(pod) {
        case Retiring:
            // Turn off other statuses
            set("Cleaning", 0);
            set("Sleeping", 0);
            set("Couch", 0);

            // Set lights
            setCouchCeiling(0);
            set("LeftVertical", 0);
            break;
            
        case Asleep:                  // Don't assume bedtime was set
            // Turn off other statuses
            set("Bedtime", 0);
            set("Cleaning", 0);
            set("Couch", 0);

            // Set lights
            setCouchCeiling(0);
            set("LeftVertical", 0);
            break;
            
        case AwakeEarly:
            //TODO: turn on nook lamp
            break;
            
        case Morning:
        case Afternoon:
            setCouchCeiling(0);
            set("LeftVertical", 0);
            break;
        case Evening:
            setCouchCeiling(20);
            set("LeftVertical", 20);
            break;
    }
}
