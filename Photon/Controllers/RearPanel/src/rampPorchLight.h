//------------
// RampPorchLight
//------------
void setRampPorchLight() {
    int percent = 0;
    int current = value("RampPorch");

    if(is("Nighttime")) {
        if(is("Outside")) {
            percent = valuem1("Outside");
        } else if(is("RampDoor") || isTimingRampDoor) {
            percent = 100;
        } else switch(partOfDay()) {
            case Evening:
                percent = 33;
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
        set("RampPorch", percent);
    }
}
