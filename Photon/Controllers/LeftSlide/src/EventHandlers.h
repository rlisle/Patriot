//------------
// Event Handlers
//------------

// Called every minute to allow delayed turn-offs, etc.
void handleNextMinute() {
    //TODO: move to motion detector
    if(isTimingLivingRoomMotion) {
        if(millis() > Device::msecsLastChange("LivingRoomMotion") + LIVINGROOM_MOTION_TIMEOUT_MSECS) {
            isTimingLivingRoomMotion = false;
            updateLights();
        }
    }
}

// Called by livingRoomMotion
void wakeupIfAfter430am() {
    if(is("sleeping") == true 
        && Time.hour() < 10 
        && ((Time.hour() == 4 && Time.minute() >= 30)
        || (Time.hour() > 4))) {
            Log.info("LS wakeupIfAfter430am");
            // Send sleeping = 0
            set("sleeping", false);     // Is this needed?
            IoT::publishValue("sleeping/set", 0);
        }
}

void handleLivingRoomMotion(int value, int oldValue) {
    Log.info("LS handleLivingRoomMotion %d", value);
    if(value > 0 && oldValue == 0) {        // Opened
        isTimingLivingRoomMotion = true;
        updateLights();
    } else if(value == 0 && oldValue > 0) { // Closed
        updateLights();
    }
}
