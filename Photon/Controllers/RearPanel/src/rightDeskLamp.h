//------------
// RightDeskLamp
//------------
void setRightDeskLamp() {
    int percent = 0;
    int current = value("RightDeskLamp");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Desk")) {
        percent = value("Desk");

    } else if(is("Office")) {
        percent = value("Office");

    } else switch(partOfDay()) {
        case AwakeEarly:
        case Evening:
            if(is("RonHome")) {
                percent = 100;
            } else {
                Log.info("Not turning on RightDeskLamp because Ron isn't home");
            }
            break;
        case Retiring:
            percent = 10;
            break;
        case Asleep:
        case Morning:
        case Afternoon:
            break;
    }
    if(percent != current) {
        set("RightDeskLamp", percent);
    }
}
