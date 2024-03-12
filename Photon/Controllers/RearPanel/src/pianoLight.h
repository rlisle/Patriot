//------------
// PianoLight
//------------
void setPianoLight() {
    int percent = 0;
    int current = value("PianoSpot");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Piano")) {
        percent = value("Piano");

    } else if(is("Office")) {
        percent = value("Office");

    } else switch(partOfDay()) {
        case Evening:
            percent = 50;
            break;
        case Retiring:
        case Asleep:
        case AwakeEarly:
        case Morning:
        case Afternoon:
            percent = 0;
    }
    if(percent != current) {
        set("PianoSpot", percent);
    }
}
