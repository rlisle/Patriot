//------------
// DoorSideLights
//------------
void setDoorSideLights() {
    int percent = 0;
    int current = value("DoorSide");

    if(is("Nighttime")) {
        if(is("Outside")) { //TODO: || is("FrontDoor") || isTimingFrontDoor) {
            percent = 100;
        } else switch(partOfDay()) {
            case Evening:
                percent = 100;
                break;
            case Retiring:
            case Asleep:
            case AwakeEarly:
            case Morning:
            case Afternoon:
                percent = 0;
        }
    }
    if(percent != current) {
        set("DoorSide", percent);
    }
}
