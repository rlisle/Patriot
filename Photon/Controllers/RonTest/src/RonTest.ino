/*
 * RonTest Controller
 *
 * Description: This sketch provides the bridge, PartOfDay, and Activities
 *
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
    Device::add(new Device("cooking"));
    Device::add(new Device("cleaning"));
    Device::add(new Device("sleeping"));
}

void loop() {
    
    IoT::loop();

    int sleeping = IoT::getChangedValue("sleeping");
    int partOfDay = IoT::getChangedValue("partofday");

    if( sleeping != -1 ) {

        Log.info("sleeping has changed: %d",sleeping);

        // Alexa, Good morning
        if( sleeping == AWAKE && partOfDay > SUNSET ) {
            setMorningLights();
        }

        // Alexa, Bedtime
        if( sleeping == RETIRING ) {
            setMorningLights();
        }

        // Alexa, Goodnight
        if( sleeping == ASLEEP ) {
            setAllInsideLights(0);
        }
    }

    if( partOfDay != -1 ) {

        Log.info("PartOfDay has changed: %d", partOfDay);

        // Turn off lights at sunrise
        if( partOfDay == SUNRISE ) {
            setAllInsideLights(0);
        }

        // Turn on lights in the evening
        if( partOfDay == DUSK ) {
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
