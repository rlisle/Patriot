//------------
// RampAwningLight
//------------
void setRampAwningLight() {
    int percent = 0;
    int current = value("RampAwning");

    if(is("Nighttime")) {
        if(is("Ramp")) {
            percent = valuem1("Ramp");
        } else if(is("Outside")){
            percent = valuem1("Outside");
        } else if(is("RampDoor") || isTimingRampDoor) {
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
        set("RampAwning", percent);
    }
}
