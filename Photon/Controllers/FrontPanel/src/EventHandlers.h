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
        if(Device::msecsLastChange("LivingRoomMotion") + FRONT_DOOR_LIGHT_TIMEOUT < msecs()) {
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
        updateLights();
    } else if(value == 0 && oldValue > 0) { // No movement
        updateLights();
    }
}
