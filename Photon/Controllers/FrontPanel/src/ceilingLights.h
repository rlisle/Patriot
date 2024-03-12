//------------
// CeilingLights
//------------
int const ceilingTheatrePercent = 15;

void setCeilingLights() {
    int percent = 0;
    int current = value("CeilingLights");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("LivingRoom")) {
        percent = value("LivingRoom");

    } else if(is("Theatre")) {
        percent = ceilingTheatrePercent;

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
    if(percent != current) {
        set("CeilingLights", percent);
    }
}
