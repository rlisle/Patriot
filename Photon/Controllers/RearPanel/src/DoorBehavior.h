//------------
// Door Behaviors
//------------
void officeDoorOpened();
void officeDoorClosed();

void handleOfficeDoor(int value, int oldValue) {
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
    officeDoorTimer = true;
}

void officeDoorClosed() {
    // Nothing to do?
}

void turnOffRearPorchAfter15mins() {
    if(officeDoorTimer == true && (millis() > msecsOfficeDoorOpened + OFFICE_DOOR_LIGHT_TIMEOUT)) {
        Device::setValue("RearPorch", 0);
        officeDoorTimer = false;
    }
}
