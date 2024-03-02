//----------------
// Event Handlers
//----------------

// Called every minute to allow delayed turn-offs, etc.
void handleNextMinute() {
    // if(isTimingFrontDoor) {
    //     if(Device::msecsLastChange("LivingRoomDoor") + FRONT_DOOR_LIGHT_TIMEOUT < millis()) {
    //         isTimingLivingRoomDoor = false;
    //         updateLights();
    //     }
    // }

    if(isTimingLivingRoomMotion) {
        if(millis() > Device::msecsLastChange("LivingRoomMotion") + FRONT_DOOR_LIGHT_TIMEOUT) {            
            isTimingLivingRoomMotion = false;
            updateLights();
        }
    }
}

// void handleFrontDoor(int value, int oldValue) {
//     Log.info("FP handleFrontDoor %d", value);
//     if(value > 0 && oldValue == 0) {        // Opened
//         updateLights();
//     } else if(value == 0 && oldValue > 0) { // Closed
//         isTimingFrontDoor = true;
//         updateLights();
//     }
// }

// Called by livingRoomMotion
void wakeupIfAfter430am() {
    if(is("sleeping") == true 
        && Time.hour() < 10 
        && ((Time.hour() == 4 && Time.minute() >= 30)
        || (Time.hour() > 4))) {
            Log.info("FP wakeupIfAfter430am");
            // Send sleeping = 0
            set("sleeping", false);     // Is this needed?
            IoT::publishValue("sleeping/set", 0);
        }
}

void handleLivingRoomMotion(int value, int oldValue) {
    if(value > 0 && oldValue == 0) {        // Movement
        Log.info("FP LivingRoomMotion");
        isTimingLivingRoomMotion = true;
        wakeupIfAfter430am();
//        updateLights();     //TODO: unneeded now?
    } else if(value == 0 && oldValue > 0) { // No movement
//        updateLights();     //TODO: unneeded now/
    }
}

// Kitchen overrides Sink
void handleKitchen(int value, int oldValue) {
    Log.info("FP Kitchen %d", value);
    if(value > 0 && oldValue == 0) {        // Turned on
//        set("SinkLamp", 0);
//        updateLights();     //TODO: unneeded now?
    }
}
