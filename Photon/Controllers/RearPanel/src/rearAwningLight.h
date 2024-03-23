//------------
// RearAwningLight
//------------
void setRearAwningLight() {
    int percent = 0;
    int current = value("RearAwning");

    if(is("Nighttime")) {
        if(is("Outside")) {
            percent = valuem1("Outside");
        } else if(is("OfficeDoor") || isTimingOfficeDoor) {
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
        set("RearAwning", percent);
    }
}
