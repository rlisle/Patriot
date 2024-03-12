//------------
// RampPorchLight
//------------
void setRampPorchLight() {
    int percent = 0;
    int current = value("RampPorch");

    if(is("Nighttime")) {
        if(is("Outside") || is("OfficeDoor")) {
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
        set("RampPorch", percent);
    }
}
