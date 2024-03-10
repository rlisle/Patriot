//------------
// Cabinets
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   Kitchen
//   LivingRoomMotion
//   Nighttime
//   Outside
//   RonHome
//   ShelleyHome
//   Sink
//   Sleeping
//------------
void setCabinets() {
    int percent = 0;

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Cabinets")) {
        percent = value("Cabinets");

    } else if(is("Theatre")) {
        percent = 0;

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
    set("Cabinets", percent);
}
