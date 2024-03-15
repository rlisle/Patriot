//------------
// officeCeilingLights
//------------
void setOfficeCeilingLights() {
    int percent = 0;
    int current = value("OfficeCeiling");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Office")) {
        percent = value("Office");

    } else switch(partOfDay()) {
        case Evening:
            percent = 50;
            break;
            
        case Retiring:
        case AwakeEarly:
            percent = 50;
            break;

        case Asleep:
        case Morning:
        case Afternoon:
            percent = 0;
    }
    if(percent != current) {
        set("OfficeCeiling", percent);
    }
}
