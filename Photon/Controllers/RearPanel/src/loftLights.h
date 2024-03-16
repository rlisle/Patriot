//------------
// LoftLights
//------------
void setLoftLights() {
    int percent = 0;
    int current = value("LoftLights");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Loft")) {
        percent = 100;

    } else switch(partOfDay()) {
        case Evening:
        case Retiring:
        case Asleep:
        case AwakeEarly:
        case Morning:
        case Afternoon:
            percent = 0;
    }
    if(percent != current) {
        set("LoftLights", percent);
    }
}
