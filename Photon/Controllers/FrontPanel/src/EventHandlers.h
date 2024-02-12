//----------------
// Event Handlers
//----------------

// Called every minute to allow delayed turn-offs, etc.
void handleNextMinute() {
    if(isTimingLivingRoomDoor) {
        if(msecsLastLivingRoomDoor < msecs() + FRONT_DOOR_LIGHT_TIMEOUT) {
            isTimingLivingRoomDoor = false;
            updateLights();
        }
    }

    if(isTimingLivingRoomMotion) {
        if(msecsLastLivingRoomMotion < msecs() + FRONT_DOOR_LIGHT_TIMEOUT) {
            isTimingLivingRoomMotion = false;
            updateLights();
        }
    }
}

void handleNighttime(int value, int oldValue) {
    Log.info("FP handleNighttime %d", value);
    if(value != oldValue) {
        nighttime = value > 0;
        updateLights();
    }
}

void handleSleeping(int value, int oldValue) {
    Log.info("FP handleSleeping %d", value);
    if(value != oldValue) {
        sleeping = value > 0;
        updateLights();
    }
}

void handleLivingRoomDoor(int value, int oldValue) {
    Log.info("FP handleLivingRoomDoor %d", value);
    if(value > 0 && oldValue == 0) {        // Opened
        livingRoomDoorOpen = true;
        msecsLastLivingRoomDoor = msecs();
        updateLights();
    } else if(value == 0 && oldValue > 0) { // Closed
        livingRoomDoorOpen = false;
        msecsLastLivingRoomDoor = msecs();
        isTimingLivingRoomDoor = true;
        updateLights();
    }
}

void handleLivingRoomMotion(int value, int oldValue) {
    Log.info("FP handleLivingRoomMotion %d", value);
    if(value > 0 && oldValue == 0) {        // Movement
        livingRoomMotion= true;
        msecsLastLivingRoomMotion = msecs();
        isTimingLivingRoomMotion = true;
        updateLights();
    } else if(value == 0 && oldValue > 0) { // No movement
        livingRoomMotion = false;
        updateLights();
    }
}

void handleRonHome(int value, int oldValue) {
    Log.info("FP handleRonHome");
    if(value != oldValue) {
        ronIsHome = value > 0;
        updateLights();
    }
}

void handleShelleyHome(int value, int oldValue) {
    Log.info("FP handleShelleyHome");
    if(value != oldValue) {
        shelleyIsHome = value > 0;
        updateLights();
    }
}
void handleAnyoneHome(int value, int oldValue) {
    Log.info("FP handleAnyoneHome");
    if(value != oldValue) {
        anyoneIsHome = value > 0;
        updateLights();
    }
}
