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

void setTestLED1(int value) {
    if(is("TestDoor")) {
        Log.info("BB TestDoor %d", testDoor);
        set("TestLED1",100);
        return;
    }
    if(isTimingTestDoor) {
        Log.info("BB Timing TestDoor");
        set("TestLED1",99);
        return;
    }
        set("TestLED1",value);
}

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

        setTestLED1(100);
        set("TestLED2", 100);
        set("TestLED3", 100);
        set("TestLED4", 100);
        return;
    }
    Log.info("BB Not cleaning");
    Log.info("BB partofday %d", partOfDay());
    switch(partOfDay()) {
        case Asleep:
            Log.info("BB updateLights Asleep");
            setTestLED1(0);
            set("TestLED2", 0);
            set("TestLED3", 0);
            set("TestLED4", 0);
            break;

        case AwakeEarly:
            Log.info("BB updateLights AwakeEarly");
            setTestLED1(50);
            set("TestLED2", 25);
            set("TestLED3", 33);
            set("TestLED4", 0);
            break;

        case Morning:
        case Afternoon:
            Log.info("BB updateLights daytime");
            setTestLED1(0);
            set("TestLED2", 0);
            set("TestLED3", 0);
            set("TestLED4", 100);
            break;

        case Evening:
            Log.info("BB updateLights evening");
            setTestLED1(50);
            set("TestLED2", 50);
            set("TestLED3", 33);
            set("TestLED4", 33);
            break;

        case Retiring:
            Log.info("BB updateLights retiring");
            setTestLED1(50);
            set("TestLED2", 0);
            set("TestLED3", 0);
            set("TestLED4", 0);
            break;
    }
    Log.info("BB updateLights exit");
}

