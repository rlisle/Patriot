//------------
// Door Behaviors
//------------
#define OFFICE_DOOR_LIGHT_TIMEOUT 15*1000

bool officeDoorOpen = false;
unsigned long msecsOfficeDoorOpened = 0;
bool isTimingOfficeDoor = false;

void officeDoorOpened();
void officeDoorClosed();

void handleOfficeDoor(int value, int oldValue) {
    Log.info("handleOfficeDoor");
    if(value > 0 && oldValue == 0) {
        officeDoorOpened();
    }
    if(value == 0 && oldValue > 0) {
        officeDoorClosed();
    }
}

void officeDoorOpened() {
    //TODO: only if at night
    Device::setValue("RearPorch", 100);
    //TODO: only if not already on
    isTimingOfficeDoor = true;
    msecsOfficeDoorOpened = millis();
}

void officeDoorClosed() {
    // Nothing to do?
}

void turnOffRearPorchAfter15mins() {
    if(isTimingOfficeDoor == true && (millis() > msecsOfficeDoorOpened + OFFICE_DOOR_LIGHT_TIMEOUT)) {
        Device::setValue("RearPorch", 0);
        isTimingOfficeDoor = false;
    }
}
