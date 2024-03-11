//------------
// BedroomLamp
//------------
void setBedroomLamp() {
    int percent = 0;
    int current = value("BedroomLamp");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Bedroom")) {
        percent = value("Bedroom");

    } else switch(partOfDay()) {
        case Evening:
        case Retiring:
            percent = 100;
            break;
        case Asleep:
        case AwakeEarly:
        case Morning:
        case Afternoon:
            percent = 0;
    }
    if(percent != current) {
        set("BedroomLamp", percent);
    }
}
