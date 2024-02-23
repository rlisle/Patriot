//------------
// BedBath Behaviors
//
// Relevant Status
//   AnyoneHome
//   Bedtime
//   Cleaning
//   Nighttime
//   RonHome
//   ShelleyHome
//   Sleeping
//------------
void updateLights() {
    Log.info("BB updateLights");

    // When cleaning is set, all inside lights are turned on
    // Turn on all outside lights also if it is nighttime
    // Assuming that not bedtime or sleeping when cleaning
    if(is("cleaning")) {
        Log.info("BB updateLights cleaning");
        // Turn off other statuses
        set("Bedtime", 0);
        set("Sleeping", 0);

        set("BlueLED", 100);
        set("TestLED1", 100);
        set("TestLED2", 100);
        set("TestLED3", 100);
        set("TestLED4", 100);
        return;
    }
    switch(partOfDay()) {
        case Asleep:
            Log.info("BB updateLights Asleep");
            set("BlueLED", 0);
            set("TestLED1", 0);
            set("TestLED2", 0);
            set("TestLED3", 0);
            set("TestLED4", 0);
            break;

        case AwakeEarly:
            Log.info("BB updateLights AwakeEarly");
            set("BlueLED", 100);
            set("TestLED1", 50);
            set("TestLED2", 25);
            set("TestLED3", 33);
            set("TestLED4", 0);
            break;

        case Morning:
        case Afternoon:
            Log.info("BB updateLights daytime");
            set("BlueLED", 0);
            set("TestLED1", 0);
            set("TestLED2", 0);
            set("TestLED3", 0);
            set("TestLED4", 100);
            break;

        case Evening:
            Log.info("BB updateLights evening");
            set("BlueLED", 100);
            set("TestLED1", 50);
            set("TestLED2", 50);
            set("TestLED3", 33);
            set("TestLED4", 33);
            break;

        case Retiring:
            Log.info("BB updateLights retiring");
            set("BlueLED", 100);
            set("TestLED1", 50);
            set("TestLED2", 0);
            set("TestLED3", 0);
            set("TestLED4", 0);
            break;
    }
}
