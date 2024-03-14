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
            if(is("RonHome")) {
                percent = 100;
            } else {
                Log.info("Not turning on OfficeCeiling because Ron isn't home");
            }
            break;
            
        case Retiring:
        case AwakeEarly:
            if(is("RonHome")) {
                percent = 50;
            } else {
                Log.info("Not turning on OfficeCeiling because Ron isn't home");
            }
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
