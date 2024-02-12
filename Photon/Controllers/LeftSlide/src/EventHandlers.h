//------------
// Event Handlers
//------------

// Called every minute to allow delayed turn-offs, etc.
void handleNextMinute() {
    if(isTimingLivingRoomMotion) {
        if(msecsLastLivingRoomMotion < msecs() + LIVINGROOM_MOTION_TIMEOUT_MSECS) {
            isTimingLivingRoomMotion = false;
            updateLights();
        }
    }

    if(isTimingLivingRoomMotion) {
        if(msecsLastLivingRoomMotion < msecs() + LIVINGROOM_MOTION_TIMEOUT_MSECS) {
            isTimingLivingRoomMotion = false;
            updateLights();
        }
    }
}

void handleNighttime(int value, int oldValue) {
    Log.info("LS handleNighttime %d", value);
    if(value != oldValue) {
        nighttime = value > 0;
        updateLights();
    }
}

void handleSleeping(int value, int oldValue) {
    Log.info("LS handleSleeping %d", value);
    if(value != oldValue) {
        sleeping = value > 0;
        updateLights();
    }
}

void handleLivingRoomMotion(int value, int oldValue) {
    Log.info("LS handleLivingRoomMotion %d", value);
    if(value > 0 && oldValue == 0) {        // Opened
        livingRoomMotion = true;
        msecsLastLivingRoomMotion = msecs();
        updateLights();
    } else if(value == 0 && oldValue > 0) { // Closed
        livingRoomMotion = false;
        msecsLastLivingRoomMotion = msecs();
        isTimingLivingRoomMotion = true;
        updateLights();
    }
}

void handleRonHome(int value, int oldValue) {
    Log.info("LS handleRonHome");
    if(value != oldValue) {
        ronIsHome = value > 0;
        updateLights();
    }
}

void handleShelleyHome(int value, int oldValue) {
    Log.info("LS handleShelleyHome");
    if(value != oldValue) {
        shelleyIsHome = value > 0;
        updateLights();
    }
}
void handleAnyoneHome(int value, int oldValue) {
    Log.info("LS handleAnyoneHome");
    if(value != oldValue) {
        anyoneIsHome = value > 0;
        updateLights();
    }
}
