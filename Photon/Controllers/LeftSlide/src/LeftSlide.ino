/**
Left Slide Controller
Description: This sketch controls the 2 lights and sensors in the left slide.
Author: Ron Lisle
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash LeftSlide"
 
    D3 HR24 sensor S1
    D4 HR24 sensor S2
    D6 DHT11/22
 
    Tx HR24 sensor Rx
    Rx HR24 sensor Tx
 
    A0 PIR sensor
    A3 Light sensor
    A5 LED 2 PWM output
    A6 (DAC) Voltage monitor R ladder
    A7 (WKP) LED1 PWM output
 
 */

#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotPIR.h>
#include <PatriotMR24.h>

#define LIVINGROOM_MOTION_TIMEOUT 2*60*1000

bool livingRoomMotion = false;
long lastLivingRoomMotion = 0;

int partOfDay = 0;
int sleeping = 0;
int watching = 0;
int cleaning = 0;

void setup() {
    IoT::begin("192.168.50.33","LeftSlide");
    createDevices();
}

void createDevices() {
    // Sensors
    Device::add(new PIR(A0, "LivingRoomMotion", "Living Room"));
    Device::add(new MR24(0, 0, "CouchPresence", "Living Room"));    // Was D3, D4

    // Lights
    Device::add(new Light(A7, "Couch", "Living Room", 2));
    Device::add(new Light(A5, "LeftVertical", "Living Room", 2));
        
    // Activities/States
    Device::add(new Device("cleaning", "All"));
    Device::add(new Device("partofday", "All"));
    Device::add(new Device("sleeping", "All"));
    Device::add(new Device("watching", "All"));
}

void loop() {
    IoT::loop();

    int cleaningChanged = Device::getChangedValue("cleaning");
    int couchPresenceChanged = Device::getChangedValue("CouchPresence");
    int livingRoomMotionChanged = Device::getChangedValue("LivingRoomMotion");
    int partOfDayChanged = Device::getChangedValue("partofday");
//    int sleepingChanged = Device::getChangedValue("sleeping");
    int watchingChanged = Device::getChangedValue("watching");
    
    long loopTime = millis();
    
    if(livingRoomMotion == true) {
        if(loopTime >= lastLivingRoomMotion+LIVINGROOM_MOTION_TIMEOUT) {
            livingRoomMotionStopped();
        }
    }

    int partOfDay = Device::value("PartOfDay");
    
    handleSleeping();
    
/*    if( sleepingChanged != -1 ) {

        Log.info("sleeping has changed %d",sleepingChanged);

        // Alexa, Good morning
        Log.info("Checking for Good Morning: sleeping: %d, partOfDay: %d",sleepingChanged,partOfDay);
        if( sleepingChanged == AWAKE) {
            Log.info("It is AWAKE");
            if(partOfDay > SUNSET || (partOfDay==0 && Time.hour() < 8)) {
                Log.info("It is morning");
                setMorningLights();
            }
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
 */

    if( partOfDayChanged != -1 ) {

        Log.info("partOfDay has changed: %d", partOfDayChanged);

        if( partOfDayChanged == SUNRISE ) {
            // Turn off lights at sunrise
            Log.info("It is sunrise");
            setSunriseLights();
        }

        if( partOfDayChanged == DUSK ) {
            // Turn on lights after sunset
            Log.info("It is dusk");
            setEveningLights();
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

    if( livingRoomMotionChanged != -1) {
        handleLivingRoomMotion(livingRoomMotionChanged, partOfDay);
    }

    if( couchPresenceChanged != -1) {        
        // Just for testing - set couch to presence value
        switch(couchPresenceChanged){
        case 25:    // presence
            Device::setValue("Couch", 20);
        case 75:    // near?
            Device::setValue("Couch", 50);
        case 100:   // Movement
            Device::setValue("Couch", 100);
        default:    // off
            //TODO: wait a minute or so
            Device::setValue("Couch", 0);
        }
        Log.info("Couch presence = %d",couchPresenceChanged);
    }

    if( cleaningChanged != -1 ) {
        if( cleaningChanged > 0 ) {
            Log.info("cleaning did turn on");
            setAllLights( 100 );
        } else {
            //TODO: check if evening lights s/b on, etc.
            Log.info("cleaning did turn off");
            setAllLights( 0 );
        }
    }
}

/**
 * handleSleeping
 *
 * Dependencies:
 *   int sleeping
 *   int partOfDay
 *   void setMorningLights()
 *   void setBedtimeLights()
 *   void setSleepingLights()
 */
void handleSleeping() {

    int sleepingChanged = Device::getChangedValue("sleeping");
    if( sleepingChanged != -1 ) {
        
        Log.info("sleeping has changed %d",sleepingChanged);

        // Alexa, Good morning
        Log.info("Checking for Good Morning: sleeping: %d, partOfDay: %d",sleepingChanged,partOfDay);
        if( sleepingChanged == AWAKE) {
            Log.info("It is AWAKE");
            if(partOfDay > SUNSET || (partOfDay==0 && Time.hour() < 8)) {
                Log.info("It is morning");
                setMorningLights();
            }
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
}

void handleLivingRoomMotion(int motion, int partOfDay) {
    if(motion > 0 ) {   // Motion detected
        lastLivingRoomMotion = millis();
        livingRoomMotion = true;
        Device::setValue("LeftVertical", 50);
        if( partOfDay > SUNSET ) {
            //TODO: maybe check sleeping already set
            if(Time.hour() > 4) {   // Motion after 5:00 is wakeup
                IoT::mqttPublish("patriot/sleeping", "1");   // AWAKE
                Device::setValue("sleeping", AWAKE);
            }
        }
    }
}

void livingRoomMotionStopped() {
    Log.info("LivingRoom motion stopped");
    livingRoomMotion = false;
    //TODO: check other things like watching, sleeping, etc.
    Device::setValue("LeftVertical", 0);
}


void setAllActivities(int value) {
    Device::setValue("cleaning", value);
    Device::setValue("watching", value);
}

void setAllLights(int value) {
    Log.info("setAllLights %d",value);
    Device::setValue("Couch", value);
    Device::setValue("LeftVertical", value);
}

void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("LeftVertical", 30);
}

void setSunriseLights() {
    Log.info("setSunriseLights");
    setAllLights(0);
}

void setEveningLights() {
    Log.info("setEveningLights");
    setAllLights(60);
}

void setBedtimeLights() {
    Log.info("setBedtimeLights");
    setAllActivities(0);
    setAllLights(0);
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    setAllActivities(0);
    setAllLights(0);
}

void setWatchingLights(int level) {
    Log.info("setWatchingLights %d", level);
    Device::setValue("Couch", 10);      // 10 and 66 don't appear to flicker
}

