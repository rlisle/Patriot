//------------
// Event Handlers
//------------

// Called every minute to allow delayed turn-offs, etc.
void handleNextMinute() {
    if(isTimingTestDoor) {
        if(millis() > Device::msecsLastChange("TestDoor") + TESTDOOR_TIMEOUT_MSECS) {
            Log.info("BB TestDoor timeout - turning off");
            isTimingTestDoor = false;
            updateLights();
        }
    }
}

void handleTestDoor(int value, int oldValue) {
    Log.info("BB handleTestDoor %d", value);
    testDoor = value;
    if(value > 0 && oldValue == 0) {        // Opened
        Log.info("BB TestDoor opened %d", testDoor);
        updateLights();
    } else if(value == 0 && oldValue > 0) { // Closed
        isTimingTestDoor = true;
        Log.info("BB TestDoor closed & timing");
        updateLights();
    }
}
