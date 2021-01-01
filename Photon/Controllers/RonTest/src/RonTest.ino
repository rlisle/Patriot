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
#include <PatriotActivity.h>
#include <PatriotPartOfDay.h>

String mqttServerIP = "192.168.10.184";

IoT     *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("RonTest");
    iot->connectMQTT(mqttServerIP, "patriotRonTest1");
    iot->begin();

    iot->addDevice(new PartOfDay());

    iot->addDevice(new Light(7, "blueLed", false, true));

    // Activities allow Alexa to control them
    // and can also turn off other activities.
    iot->addDevice(new Activity("cooking"));
    iot->addDevice(new Activity("cleaning"));
    iot->addDevice(new Activity("sleeping"));
    
    // Initialize any states we'll be testing in loop
    iot->setStateValue("sleeping", 0);
    iot->setStateValue("partofday", 0);
}

void loop() {
    iot->loop();
    
    
    State* sleeping = iot->getState("sleeping");
    State* partOfDay = iot->getState("partofday");

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
    }

    if( partOfDay != NULL && partOfDay->hasChanged() ) {

        Log.info("PartOfDay has changed: %d", partOfDay->value());

        if( partOfDay->value() == SUNRISE ) {
            setAllInsideLights(0);
        }

        if( partOfDay->value() == DUSK ) {
            setEveningLights();
        }
    }
}

void setMorningLights() {
    iot->setDeviceValue("BlueLED", 100);
}

void setEveningLights() {
    iot->setDeviceValue("BlueLED", 0);
}

void setAllInsideLights(int level) {
    iot->setDeviceValue("BlueLED", level);
}
