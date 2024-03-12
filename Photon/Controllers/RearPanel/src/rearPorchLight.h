//------------
// RearPorchLight
//------------
void setRearPorchLight() {
    int percent = 0;
    int current = value("RearPorch");

    if(is("Nighttime")) {
        if(is("Outside") || is("OfficeDoor") || isTimingOfficeDoor) {
            set("RearPorch", 100);
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
        set("PianoSpot", percent);
    }
}
