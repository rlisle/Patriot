//------------
// Behaviors
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   LivingRoomMotion
//   Nighttime
//   RonHome
//   ShelleyHome
//   Sleeping
//------------

#define LIVINGROOM_MOTION_TIMEOUT_MSECS 15*1000

// Update all devices managed by this Photon2
void updateLights() {
    PartOfDay pod = partOfDay();

    // Get pointers to all devices
//    Device *couchCeiling = Device::get("CouchCeiling");
//    Device *leftVertical = Device::get("LeftVertical");

    // When cleaning is set, all inside lights are turned on
    // Turn on all outside lights also if it is nighttime
    // Assuming that not bedtime or sleeping when cleaning
    if(is("Cleaning")) {
        // Turn off other statuses
        set("Bedtime", 0);
        set("Sleeping", 0);

        // Set lights
        set("CouchCeiling",100);
        set("LeftVertical", 100);
        return;
    }

    switch(pod) {
        case Retiring:
            // Turn off other statuses
            set("Cleaning", 0);
            set("Sleeping", 0);

            // Set lights
            set("CouchCeiling", 0);
            set("LeftVertical", 0);
            break;
            
        case Asleep:                  // Don't assume bedtime was set
            // Turn off other statuses
            set("Bedtime", 0);
            set("Cleaning", 0);

            // Set lights
            set("CouchCeiling", 0);
            set("LeftVertical", 0);
            break;
            
        case AwakeEarly:
            //TODO: turn on nook lamp
            break;
            
        case Morning:
        case Afternoon:
            set("CouchCeiling", 0);
            set("LeftVertical", 0);
            break;
        case Evening:
            set("CouchCeiling", 20);
            set("LeftVertical", 20);
            break;
    }
}
