//---------------
// Handle Events
//---------------

// Called every minute to allow delayed turn-offs, etc.
void handleNextMinute() {
    if(isTimingOfficeDoor) {
        if(millis() > Device::msecsLastChange("OfficeDoor") + OFFICE_DOOR_TIMEOUT_MSECS) {
            Log.info("RP turning off isTimingOfficeDoor");
            isTimingOfficeDoor = false;
            updateLights();
        }
    }

    if(isTimingOfficeMotion) {
        if(Device::msecsLastChange("OfficeMotion") + OFFICE_MOTION_TIMEOUT_MSECS < millis()) {
            isTimingOfficeMotion = false;
            updateLights();
        }
    }
}

void handleOfficeDoor(int value, int oldValue) {
    Log.info("RP handleOfficeDoor %d", value);
    if(value > 0 && oldValue == 0) {        // Opened
        Log.info("RP door opened");
//        isTimingOfficeDoor = false;
//        updateLights();
    } else if(value == 0 && oldValue > 0) { // Closed
        Log.info("RP door closed, setting isTimingOfficeDoor");
        isTimingOfficeDoor = true;
//        updateLights();
    }
//    Log.info("RP handleOfficeDoor exit");
}

void handleOfficeMotion(int value, int oldValue) {
    Log.info("RP handleOfficeMotion %d", value);
    if(value > 0 && oldValue == 0) {        // Movement
        isTimingOfficeMotion = true;
//        updateLights();
    } else if(value == 0 && oldValue > 0) { // No movement
//        updateLights();
    }
}

void handleLivingRoomMotion(int value, int oldValue) {
    Log.info("RP handleLivingRoomMotion %d", value);
    if(value > 0 && oldValue == 0) {        // Movement
        // Nothing to do. May want to track motion from office to LR and vs. a vs.
    } else if(value == 0 && oldValue > 0) { // No movement
        // Nothing to do.
    }
}
