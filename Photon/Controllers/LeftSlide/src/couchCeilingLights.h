//----------------------
// CouchCeilingLights
//----------------------
void setCouchCeilingLights() {
    int percent = 0;
    int current = value("CouchCeiling");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Couch")) {
        percent = valuem1("Couch");

    } else if(is("Theatre")) {
        percent = 33;

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
        set("CouchCeiling", percent);
    }
}