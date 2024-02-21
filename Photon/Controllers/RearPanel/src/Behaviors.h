//------------
// Behaviors
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   Desk
//   LivingRoomMotion
//   Nighttime
//   Office
//   Outside
//   RonHome
//   ShelleyHome
//   Sleeping
//------------
#define OFFICE_DOOR_LIGHT_TIMEOUT 15*1000

// Update all devices managed by this Photon2
void updateLights() {
    Log.info("RP updateLights");

    // When cleaning is set, all inside lights are turned on
    // Turn on all outside lights also if it is nighttime
    // Assuming that not bedtime or sleeping when cleaning
    if(is("cleaning")) {
        Log.info("RP updateLights cleaning");
        // Turn off other statuses
        set("Bedtime", 0);
        set("Sleeping", 0);

        set("Curtain", 100);        // Open curtain
        set("Loft", 100);
        set("OfficeCeiling", 100);
        set("Piano", 100);
        return;
    }
    switch(partOfDay()) {
        case Asleep:
            Log.info("RP updateLights Asleep");
            set("Curtain", 0);     // Close curtain

            set("OfficeCeiling", 5);
            set("Loft", 0);
            set("Piano", 5);

            set("RampPorch", 0);
            set("RampAwning", 0);
            set("RearAwning", 0);
            if(is("officeDoor") || isTimingOfficeDoor) {        
                set("RearPorch", 100);
            } else {
                set("RearPorch", 0);
            }
            break;

        case AwakeEarly:
            Log.info("RP updateLights AwakeEarly");
            set("OfficeCeiling", 5);
            set("Loft", 0);
            set("Piano", 5);

            set("RampPorch", 0);
            set("RampAwning", 0);
            set("RearAwning", 0);
            if(is("officeDoor") || isTimingOfficeDoor) {        
                Log.info("RP AwakeEarly door open");
                set("RearPorch", 100);
            } else {
                set("RearPorch", 0);
            }
            break;

        case Morning:
        case Afternoon:
            Log.info("RP updateLights daytime");
            set("OfficeCeiling", 0);
            set("Loft", 0);
            set("Piano", 100);

            set("RampPorch", 0);
            set("RampAwning", 0);
            set("RearPorch", 0);
            set("RearAwning", 0);
            break;

        case Evening:
            Log.info("RP updateLights evening");
            set("OfficeCeiling", 5);
            set("Loft", 0);
            set("Piano", 100);

            set("RampPorch", 100);
            set("RampAwning", 100);
            set("RearPorch", 100);
            set("RearAwning", 100);
            break;

        case Retiring:
            Log.info("RP updateLights retiring");

            set("Curtain", 0);     // Close curtain
            
            set("OfficeCeiling", 5);
            set("Loft", 0);
            set("Piano", 5);

            set("RampPorch", 0);
            set("RampAwning", 0);
            set("RearPorch", 0);
            set("RearAwning", 0);
            if(is("officeDoor") || isTimingOfficeDoor) {        
                set("RearPorch", 100);
            }
            break;
    }
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