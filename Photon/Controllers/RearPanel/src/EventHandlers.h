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

void handleCleaning(int value, int oldValue) {
    Log.info("RP handleCleaning %d", value);
    if(value != oldValue) {
        cleaning = value > 0;
        updateLights();
    }
}

void handleNighttime(int value, int oldValue) {
    Log.info("RP handleNighttime %d", value);
    if(value != oldValue) {
        nighttime = value > 0;
        updateLights();
    }
}

void handleSleeping(int value, int oldValue) {
    Log.info("RP handleSleeping %d", value);
    if(value != oldValue) {
        sleeping = value > 0;
        updateLights();
    }
}

void handleOfficeDoor(int value, int oldValue) {
    Log.info("RP handleOfficeDoor %d", value);
    if(value > 0 && oldValue == 0) {        // Opened
        officeDoorOpen = true;
        msecsLastDoorEvent = msecs();
        updateLights();
    } else if(value == 0 && oldValue > 0) { // Closed
        officeDoorOpen = false;
        msecsLastDoorEvent = msecs();
        isTimingOfficeDoor = true;
        updateLights();
    }
}

void handleOfficeMotion(int value, int oldValue) {
    Log.info("RP handleOfficeMotion %d", value);
    if(value > 0 && oldValue == 0) {        // Movement
        officeMotion= true;
        msecsLastOfficeMotion = msecs();
        isTimingOfficeMotion = true;
        updateLights();
    } else if(value == 0 && oldValue > 0) { // No movement
        officeMotion = false;
        updateLights();
    }
}

void handleRonHome(int value, int oldValue) {
    Log.info("RP handleRonHome");
    if(value != oldValue) {
        ronIsHome = value > 0;
        updateLights();
    }
}

void handleShelleyHome(int value, int oldValue) {
    Log.info("RP handleShelleyHome");
    if(value != oldValue) {
        shelleyIsHome = value > 0;
        updateLights();
    }
}
void handleAnyoneHome(int value, int oldValue) {
    Log.info("RP handleAnyoneHome");
    if(value != oldValue) {
        anyoneIsHome = value > 0;
        updateLights();
    }
}
