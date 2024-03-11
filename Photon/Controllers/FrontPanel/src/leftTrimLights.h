//------------
// LeftTrimLights
//------------
void setLeftTrimLights() {
    int percent = 0;
    int current = value("LeftTrimLights");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("LivingRoom")) {
        percent = value("LivingRoom");

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
    if(percent != current) {
        set("LeftTrimLights", percent);
    }
}
