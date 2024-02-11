//---------------
// Handle Events
//---------------

// Called every minute to allow delayed turn-offs, etc.
void handleNextMinute() {
    if(isTimingOfficeDoor) {
        if(msecsLastDoorEvent < msecs() + OFFICE_DOOR_TIMEOUT_MSECS) {
            isTimingOfficeDoor = false;
            updateLights();
        }
    }

    if(isTimingOfficeMotion) {
        if(msecsLastOfficeMotion < msecs() + OFFICE_MOTION_TIMEOUT_MSECS) {
            isTimingOfficeMotion = false;
            updateLights();
        }
    }
}

void handleNighttime(int value, int oldValue) {
    Log.info("handleNighttime %d", value);
    if(value != oldValue) {
        nighttime = value > 0;
        updateLights();
    }
}

void handleSleeping(int value, int oldValue) {
    Log.info("handleSleeping %d", value);
    if(value != oldValue) {
        sleeping = value > 0;
        updateLights();
    }
}

void handleOfficeDoor(int value, int oldValue) {
    Log.info("handleOfficeDoor %d", value);
    if(value > 0 && oldValue == 0) {        // Opened
        officeDoorOpen = true;
        msecsLastDoorEvent = msecs();
        updateLights();
    } else if(value == 0 && oldValue > 0) { // Closed
        officeDoorOpen = false;
        msecsLastDoorEvent = msecs();
        updateLights();
    }
}

void handleRonHome(int value, int oldValue) {
    Log.info("handleRonHome");
    if(value != oldValue) {
        ronIsHome = value > 0;
        updateLights();
    }
}

void handleShelleyHome(int value, int oldValue) {
    Log.info("handleShelleyHome");
    if(value != oldValue) {
        shelleyIsHome = value > 0;
        updateLights();
    }
}
void handleAnyoneHome(int value, int oldValue) {
    Log.info("handleAnyoneHome");
    if(value != oldValue) {
        anyoneIsHome = value > 0;
        updateLights();
    }
}
