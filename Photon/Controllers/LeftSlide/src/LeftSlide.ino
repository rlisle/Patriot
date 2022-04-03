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

int watching = 0;
int cleaning = 0;
int couchPresence = 0;

// Move to IoT eventually
int partOfDay = 0;
int sleeping = 0;


void setup() {
    IoT::begin("192.168.50.33","LeftSlide");
    createDevices();
    handleDaylightSavings();
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
    Device::add(new Device("watching", "All"));
    
    //Move to IoT eventually
    Device::add(new Device("partofday", "All"));
    Device::add(new Device("sleeping", "All"));

}

// TODO: refactor to IoT
// from 2nd Sunday of March through 1st Sunday of November
// 2022 3/13 - 11/6, 2023 3/12 - 11/5, 2024 3/10 - 11/3
void handleDaylightSavings() {
    int month = Time.month();
    if(month > 3 && month < 11) {
        Time.beginDST();
    } else if(month == 3) {
        handleDSTMarch();
    } else if(month == 11) {
        handleDSTNovember();
    }
}

void handleDSTMarch() {
    int weekday = Time.weekday();
    int day = Time.day();
    int hour = Time.hour();
    
    if(day <= 7) return;
    
    switch(weekday) {
        case 1:     // Sunday
            if(day == 8 && hour < 2) return;
            break;
        case 2:
            if(day < 9) return;
        case 3:
            if(day < 10) return;
        case 4:
            if(day < 11) return;
        case 5:
            if(day < 12) return;
        case 6:
            if(day < 13) return;
        case 7:     // Saturday
        default:
            if(day < 14) return;
    }
    Time.beginDST();
}

void handleDSTNovember() {
    int weekday = Time.weekday();
    int day = Time.day();
    int hour = Time.hour();
    
    if(day > 7) return;
    
    switch(weekday) {
        case 1:     // Sunday
            if(day == 1 && hour >= 2) return;
            break;
        case 2:
            if(day > 2) return;
            break;
        case 3:
            if(day > 3) return;
            break;
        case 4:
            if(day > 4) return;
            break;
        case 5:
            if(day > 5) return;
            break;
        case 6:
            if(day > 6) return;
            break;
        case 7:     // Saturday
        default:
            if(day > 7) return;
    }
    Time.beginDST();
}


void loop() {
    IoT::loop();

    handlePartOfDay();
    handleSleeping();
    handleLivingRoomMotion();
    handleWatching();
    handleCleaning();
    handleCouchPresence();
}

/**
 * handlePartOfDay
 *
 * Dependencies:
 *   int partOfDay
 *   void setSunriseLights()
 *   void setEveningLights()
 */
void handlePartOfDay() {
    
    int partOfDayChanged = Device::getChangedValue("partofday");
    if( partOfDayChanged != -1 ) {

        Log.info("partOfDay has changed: %d", partOfDayChanged);

        if( partOfDayChanged == SUNRISE ) {
            Log.info("It is sunrise");
            setSunriseLights();
        }

        if( partOfDayChanged == DUSK ) {
            Log.info("It is dusk");
            setEveningLights();
        }
        partOfDay = partOfDayChanged;
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
        
        sleeping = sleepingChanged;
    }
}

void handleLivingRoomMotion() {

    // Timed shut-off
    long loopTime = millis();
    if(livingRoomMotion == true) {
        if(loopTime >= lastLivingRoomMotion+LIVINGROOM_MOTION_TIMEOUT) {
            Log.info("LivingRoom motion stopped");
            livingRoomMotion = false;
            //TODO: check other things like watching, sleeping, etc.
            Device::setValue("LeftVertical", 0);
        }
    }

    int livingRoomMotionChanged = Device::getChangedValue("LivingRoomMotion");
    if(livingRoomMotionChanged > 0 ) {         // Motion?
        livingRoomMotion = true;
        lastLivingRoomMotion = millis();
        
        Device::setValue("LeftVertical", 50);
        
        // Determine if this is Ron getting up
        if( partOfDay > SUNSET && sleeping > 1) {
            if(Time.hour() > 4) {   // Motion after 5:00 is wakeup
                IoT::mqttPublish("patriot/sleeping", "1");   // AWAKE
                Device::setValue("sleeping", AWAKE);
            }
        }
    }
}

/**
 * handleWatching
 *
 * Dependencies:
 *   int partOfDay
 *   void setWatchingLights()
 */
void handleWatching() {
    
    int watchingChanged = Device::getChangedValue("watching");
    if( watchingChanged != -1 ) {
        if( watchingChanged > 0 ) {
            watching = 100;
            Log.info("watching did turn on");
            Device::setValue("Couch", 10);      // 10 and 66 don't appear to flicker

        } else {
            watching = 0;
            //TODO: check if evening lights s/b on, etc.
            Log.info("watching did turn off");
            Device::setValue("Couch", 0);
        }
    }
}

void handleCleaning() {
    int cleaningChanged = Device::getChangedValue("cleaning");
    if( cleaningChanged != -1 ) {
        if( cleaningChanged > 0 ) {
            cleaning = 100;
            Log.info("cleaning did turn on");
            //TODO: save current light state to restore when cleaning turned off
            setAllLights( 100 );
        } else {
            cleaning = 0;
            //TODO: check if evening lights s/b on, etc.
            Log.info("cleaning did turn off");
            setAllLights( 0 );
        }
    }
}

void handleCouchPresence() {
    int couchPresenceChanged = Device::getChangedValue("CouchPresence");
    if( couchPresenceChanged != -1) {
        couchPresence = couchPresenceChanged;
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
        Log.info("Couch presence = %d",couchPresence);
    }
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
