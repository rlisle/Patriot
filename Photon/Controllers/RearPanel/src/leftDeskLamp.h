//------------
// LeftDeskLamp
//------------
void setLeftDeskLamp() {
    int percent = 0;
    int current = value("LeftDeskLamp");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Desk")) {
        percent = value("Desk");

    } else if(is("Office")) {
        percent = value("Office");

    } else switch(partOfDay()) {
        case AwakeEarly:
        case Evening:
        case Morning:
        case Afternoon:
            if(is("RonHome")) {
                percent = 100;
            } else {
                Log.info("Not turning on LeftDeskLamp because Ron isn't home");
            }
            break;
        case Retiring:
            percent = 33;
            break;
        case Asleep:
            percent = 0;
    }
    if(percent != current) {
        set("LeftDeskLamp", percent);
    }
}
