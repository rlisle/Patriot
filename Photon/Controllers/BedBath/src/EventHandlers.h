//------------
// Event Handlers
//------------

// Called every minute to allow delayed turn-offs, etc.
void handleNextMinute() {
    Log.info("handleNextMinute");
    //TODO: Generalize this so it can be reused by any device
    // if(isTimingLivingRoomMotion) {
    //     if(msecs() > Device::msecsLastChange("LivingRoomMotion") + LIVINGROOM_MOTION_TIMEOUT_MSECS) {
    //         isTimingLivingRoomMotion = false;
    //         updateLights();
    //     }
    // }
}

// void handleLivingRoomMotion(int value, int oldValue) {
//     Log.info("LS handleLivingRoomMotion %d", value);
//     if(value > 0 && oldValue == 0) {        // Opened
//         isTimingLivingRoomMotion = true;
//         updateLights();
//     } else if(value == 0 && oldValue > 0) { // Closed
//         updateLights();
//     }
// }
