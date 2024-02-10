//------------
// Home Behaviors
//------------
bool ronIsHome = true;
bool shelleyIsHome = true;
bool anyoneIsHome = true;

void handleRonHome(int value, int oldValue) {
    Log.info("handleRonHome");
    if(value > 0 && oldValue == 0) {
        ronIsHome = true;
    }
    if(value == 0 && oldValue > 0) {
        ronIsHome = false;
    }
}

void handleShelleyHome(int value, int oldValue) {
    Log.info("handleShelleyHome");
    if(value > 0 && oldValue == 0) {
        shelleyIsHome = true;
    }
    if(value == 0 && oldValue > 0) {
        shelleyIsHome = false;
    }
}

void handleAnyoneHome(int value, int oldValue) {
    Log.info("handleAnyoneHome");
    if(value > 0 && oldValue == 0) {
        anyoneIsHome = true;
    }
    if(value == 0 && oldValue > 0) {
        anyoneIsHome = false;
    }
}
