//------------
// NookLamp
//------------
const int nookTheatrePercent = 50;

void setNookLamp() {
    int percent = 0;
    int current = value("NookLamp");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Nook")) {
        percent = value("Nook");

    } else if(is("Theatre")) {
        percent = nookTheatrePercent;

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
        set("NookLamp", percent);
    }
}
