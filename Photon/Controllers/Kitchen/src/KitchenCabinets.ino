/**
KitchenCabinets Controller
Description: This sketch controls all the lights on the kitchen cabinets.
Author: Ron Lisle

 The controller is located above the kitchen cabinets.
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash Kitchen"

  To setup a new controller, be sure to "particle library add <libname>" for each
  included library. Currently this controller uses IoT and PatriotNCD16Dimmer.
 
 */
#include <IoT.h>
#include <PatriotNCD16Dimmer.h>

#define ADDRESS 0x41      // PWM board address A0 jumper set

void setup() {
    IoT::begin("192.168.50.33", "Kitchen");

    // Cabinet Lights
    Device::add(new NCD16Dimmer(ADDRESS, 0, "Top1", "Kitchen", 2)); // Top front
    Device::add(new NCD16Dimmer(ADDRESS, 1, "Top2", "Kitchen", 2));
    Device::add(new NCD16Dimmer(ADDRESS, 2, "Top3", "Kitchen", 2)); // Rear left
    Device::add(new NCD16Dimmer(ADDRESS, 3, "Top4", "Kitchen", 2)); // Rear right
    
    Device::add(new NCD16Dimmer(ADDRESS, 4, "Mid1", "Kitchen", 2)); // Nearest Sink
    Device::add(new NCD16Dimmer(ADDRESS, 5, "Mid2", "Kitchen", 2)); // Over Window
    Device::add(new NCD16Dimmer(ADDRESS, 6, "Mid3", "Kitchen", 2)); // Over Coffee Pot
    Device::add(new NCD16Dimmer(ADDRESS, 7, "Mid4", "Kitchen", 2));
    Device::add(new NCD16Dimmer(ADDRESS, 8, "Mid5", "Kitchen", 2));

    Device::add(new NCD16Dimmer(ADDRESS, 9, "Bottom1", "Kitchen", 2));  // Rear ledge

    // Activities/States - define for every other state
    // Be careful to only define in 1 (this) controller (Is this still true? I don't think so)
    Device::add(new Device("cleaning", "All"));
    Device::add(new Device("cooking", "All"));
    Device::add(new Device("partofday", "All"));
    Device::add(new Device("sleeping", "All"));
    Device::add(new Device("watching", "All"));
}

void loop() {

    IoT::loop();
    
    int cleaningChanged = Device::getChangedValue("cleaning");
    int cookingChanged = Device::getChangedValue("cooking");
    int partOfDayChanged = Device::getChangedValue("partofday");
    int sleepingChanged = Device::getChangedValue("sleeping");
    int watchingChanged = Device::getChangedValue("watching");

    if( sleepingChanged != -1 ) {

        Log.info("sleeping has changed %d",sleepingChanged);
        
        int partOfDay = Device::value("PartOfDay");

        // Alexa, Good morning
        Log.info("Checking for Good Morning: sleeping: %d, partOfDay: %d",sleepingChanged,partOfDay);
        if( sleepingChanged == AWAKE && partOfDay > SUNSET ) {
            Log.info("It is good morning");
            setMorningLights();
        }

        // Alexa, Bedtime
        if( sleepingChanged == RETIRING ) {
            setBedtimeLights();
        }

        // Alexa, Goodnight
        if( sleepingChanged == ASLEEP ) {
            setSleepingLights();
        }
    }

    if( cleaningChanged != -1 ) {
        if( cleaningChanged > 0 ) {
            Log.info("cleaning did turn on");
            setAllInsideLights( 100 );
        } else {
            //TODO: check if evening lights s/b on, etc.
            Log.info("cleaning did turn off");
            setAllInsideLights( 0 );
        }
    }

    if( watchingChanged != -1 ) {
        if( watchingChanged > 0 ) {
            Log.info("watching did turn on");
            setWatchingLights( 100 );
        } else {
            //TODO: check if evening lights s/b on, etc.
            Log.info("watching did turn off");
            setWatchingLights( 0 );
        }
    }
}

void setAllActivities(int value) {
    Device::setValue("cooking", value);
    Device::setValue("cleaning", value);
    Device::setValue("watching", value);
}

void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("Mid1",70);
    Device::setValue("Mid2",70);
    Device::setValue("Mid3",100);    // Over coffee pot
    Device::setValue("Top4",70);
    Device::setValue("Top5",70);
}

void setSunriseLights() {
    Log.info("setSunriseLights");
    setAllInsideLights(0);
}

void setEveningLights() {
    Log.info("setEveningLights");
    setAllInsideLights(70);
}

void setBedtimeLights() {
    Log.info("setBedtimeLights");
    setAllActivities(0);
    setAllInsideLights(0);
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    setAllActivities(0);
    setAllInsideLights(0);
}

void setWatchingLights(int level) {
    Log.info("setWatchingLights %d", level);
    setAllInsideLights(70);
    //TODO:
}

void setAllInsideLights(int value) {
    Log.info("setAllInsideLights %d",value);
    Device::setValue("Top1", value);
    Device::setValue("Top2", value);
    Device::setValue("Top3", value);
    Device::setValue("Top4", value);
    Device::setValue("Mid1", value);
    Device::setValue("Mid2", value);
    Device::setValue("Mid3", value);
    Device::setValue("Mid4", value);
    Device::setValue("Mid5", value);
    Device::setValue("Bottom1", value);
}
