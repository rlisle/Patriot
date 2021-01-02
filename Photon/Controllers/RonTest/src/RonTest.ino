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

unsigned long lastTime = 0;

void loop() {
    unsigned long currentTime;
    
    IoT::loop();

    // Do this every 15 seconds
    currentTime = millis();
    if( currentTime > lastTime + 15000 ) {
    
        lastTime = currentTime;
        
        Device* sleeping = Device::get("sleeping");
        Device* partOfDay = Device::get("partofday");

        if( sleeping != NULL && sleeping->hasChanged() ) {

            Log.info("sleeping has changed: %d",sleeping->value());

            // Alexa, Good morning
            if( sleeping->value() == AWAKE && partOfDay->value() > SUNSET ) {
                setMorningLights();
            }

            // Alexa, Bedtime
            if( sleeping->value() == RETIRING ) {
                setMorningLights();
            }

            // Alexa, Goodnight
            if( sleeping->value() == ASLEEP ) {
                setAllInsideLights(0);
            }
            
            sleeping->syncPrevious();
        }

        if( partOfDay != NULL && partOfDay->hasChanged() ) {

            Log.info("PartOfDay has changed: %d", partOfDay->value());

            // Turn off lights at sunrise
            if( partOfDay->value() == SUNRISE ) {
                setAllInsideLights(0);
            }

            // Turn on lights in the evening
            if( partOfDay->value() == DUSK ) {
                setEveningLights();
            }
            
            partOfDay->syncPrevious();
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
