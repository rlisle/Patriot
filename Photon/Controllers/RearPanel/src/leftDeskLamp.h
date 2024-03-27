//------------
// LeftDeskLamp
//------------
void setLeftDeskLamp() {
    int percent = 0;
    int current = value("LeftDeskLamp");
    Log.info("Current LeftDeskLamp %d",current);

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Desk")) {
        percent = valuem1("Desk");
        Log.info("Desk override LeftDeskLamp %d",percent);

    } else if(is("Office")) {
        percent = valuem1("Office");

    } else switch(partOfDay()) {
        case AwakeEarly:
        case Evening:
        case Morning:
        case Afternoon:
            // if(is("RonHome")) {
                 percent = 100;
            // } else {
            //    Log.info("Not turning on LeftDeskLamp because Ron isn't home");
            //}
            break;
        case Retiring:
            percent = 10;
            break;
        case Asleep:
            percent = 0;
    }
    //TODO: is this needed?
//    if(percent != current) {
        Log.info("Setting LeftDeskLamp %d",percent);
        set("LeftDeskLamp", percent);
//    }
}
