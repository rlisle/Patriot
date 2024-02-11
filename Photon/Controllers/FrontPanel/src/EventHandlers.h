//----------------
// Event Handlers
//----------------

//TODO: Refactor and generalize all this
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
