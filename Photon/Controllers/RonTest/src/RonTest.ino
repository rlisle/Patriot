/*
 * RonTest Controller
 *
 * Description: This sketch provides the bridge, PartOfDay, and Activities
 *
 * PartOfDay Values
 * 0 SUNRISE
 * 1 MORNING
 * 2 NOON
 * 3 AFTERNOON
 * 4 SUNSET
 * 5 DUSK
 * 6 NIGHT
 * 7 DAWN

 * Author: Ron Lisle
 *
 * Hardware
 * - built-in blue LED     D7
 */

#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotPartOfDay.h>

void setup() {
    IoT::begin("192.168.10.184", "RonTest");

    Device::add(new PartOfDay());

    Device::add(new Light(7, "blueLed", false, true));

    // Basic devices allow Alexa to control the name
    // and can also turn off other activities.
    Device::add(new Device("cleaning"));
    Device::add(new Device("sleeping"));
}

void loop() {
    
    IoT::loop();

    int changedSleeping = Device::getChangedValue("sleeping");
    int changedPartOfDay = Device::getChangedValue("partofday");
    int sleeping = Device::value("sleeping");
    int partOfDay = Device::value("partofday");

    if( changedSleeping != -1 ) {
        
        Log.info("sleeping has changed: %d",sleeping);

        // Alexa, Good morning
        Log.info("Checking for Good Morning: sleeping: %d, partOfDay: %d",sleeping,partOfDay);
        if( changedSleeping == AWAKE && partOfDay > SUNSET ) {
            Log.info("It is good morning");
            setMorningLights();
        }

        // Alexa, Bedtime
        if( changedSleeping == RETIRING ) {
            setMorningLights();
        }

        // Alexa, Goodnight
        if( changedSleeping == ASLEEP ) {
            setAllInsideLights(0);
        }
    }

    if( changedPartOfDay != -1 ) {

        Log.info("PartOfDay has changed: %d", partOfDay);

        // Turn off lights at sunrise
        if( changedPartOfDay == SUNRISE ) {
            setAllInsideLights(0);
        }

        // Turn on lights in the evening
        if( changedPartOfDay == DUSK ) {
            setEveningLights();
        }
    }
}

void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("BlueLED", 100);
}

void setEveningLights() {
    Log.info("setEveningLights");
    Device::setValue("BlueLED", 100);
}

void setAllInsideLights(int value) {
    Log.info("setAllInsideLights %d",value);
    Device::setValue("BlueLED", value);
}
