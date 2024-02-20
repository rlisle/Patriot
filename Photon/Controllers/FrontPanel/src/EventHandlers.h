//----------------
// Event Handlers
//----------------

// Called every minute to allow delayed turn-offs, etc.
void handleNextMinute() {
    // if(isTimingFrontDoor) {
    //     if(Device::msecsLastChange("LivingRoomDoor") + FRONT_DOOR_LIGHT_TIMEOUT < msecs()) {
    //         isTimingLivingRoomDoor = false;
    //         updateLights();
    //     }
    // }

    if(isTimingLivingRoomMotion) {
        if(msecs() > Device::msecsLastChange("LivingRoomMotion") + FRONT_DOOR_LIGHT_TIMEOUT) {
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

void handleLivingRoomMotion(int value, int oldValue) {
    Log.info("FP handleLivingRoomMotion %d", value);
    if(value > 0 && oldValue == 0) {        // Movement
        isTimingLivingRoomMotion = true;
        updateLights();     //TODO: unneeded now?
    } else if(value == 0 && oldValue > 0) { // No movement
        updateLights();     //TODO: unneeded now/
    }
}

// Kitchen overrides Sink
void handleKitchen(int value, int oldValue) {
    Log.info("FP handleKitchen %d", value);
    if(value > 0 && oldValue == 0) {        // Turned on
        set("Sink", 0);
        updateLights();     //TODO: unneeded now?
    }
}
