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

String mqttServerIP = "192.168.10.184";

IoT     *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("RonTest");
    iot->connectMQTT(mqttServerIP, "patriotRonTest1");
    iot->begin();

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
    
    iot->loop();

    // Do this every 15 seconds
    currentTime = millis();
    if( currentTime > lastTime + 15000 ) {
    
        lastTime = currentTime;
        
        Device* sleeping = Device::get("sleeping");
        Device* partOfDay = Device::get("partofday");

        if( sleeping != NULL ) {
            
            if( sleeping->hasChanged() ) {

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
        }

        if( partOfDay != NULL && partOfDay->hasChanged() ) {

            Log.info("PartOfDay has changed: %d", partOfDay->value());

            if( partOfDay->value() == SUNRISE ) {
                setAllInsideLights(0);
            }

            if( partOfDay->value() == DUSK ) {
                setEveningLights();
            }
            
            partOfDay->syncPrevious();
        }
    }
}

void setMorningLights() {
    Device::setValue("BlueLED", 100);
}

void setEveningLights() {
    Device::setValue("BlueLED", 0);
}

void setAllInsideLights(int value) {
    Device::setValue("BlueLED", value);
}
