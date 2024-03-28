//------------
// NookLamp
//------------
const int nookTheatrePercent = 50;

void setNookLamp() {
    int percent = 0;
    int current = value("NookLamp");
    Log.info("NookLamp currently %d", current);

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Nook")) {
        percent = value("Nook");
        Log.info("Nook override setting %d", percent);

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
            Log.info("Nook POD setting off");
            percent = 0;
    }
    if(percent != current) {
        Log.info("Updating Nook from %d to %d", current, percent);
        set("NookLamp", percent);
    } else {
        Log.info("Nook same");
    }
}
