//------------
// Behaviors
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
void setBedroom() {
    int percent = 0;

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
    set("BedroomLamp", percent);
}
