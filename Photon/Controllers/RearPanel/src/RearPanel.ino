/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle

 The controller is located in the office above the piano.
 It includes the PartOfDay plugin.
 
 Photon pin assignments
 - A2-A4 future 12v monitor ?
 - A5 PIR
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash RearPanel"
 
 TODO: Add GPS board (Rx, Vin, Gnd)
 
 Using SYSTEM_THREAD(ENABLED) is recommended,
 and runs network on separate theread.
 Using SYSTEM_MODE(SEMI_AUTOMATIC) we will
 manually connect, but everything is automatic
 after that. This allows running loop and MQTT
 even if no internet available

 */
#include <IoT.h>
#include <PatriotNCD8Light.h>
#include <PatriotPartOfDay.h>
#include <PatriotCurtain.h>
#include <PatriotNCD4Switch.h>
#include <PatriotNCD4Relay.h>
#include <PatriotPIR.h>
//#include "secrets.h"   // Modify this to include your passwords: HUE_USERID

#define OFFICE_MOTION_TIMEOUT 2*60*1000
#define OFFICE_DOOR_TIMEOUT 5*60*1000

#define ADDRESS 1      // PWM board address A0 jumper set
#define I2CR4IO4 0x20  // 4xRelay+4GPIO address

// Resellable Switch Wiring
// In order to keep the RV resellable, switches need to work without IoT.
// So changing switches from inputs to directly control LEDs.
// Moved from A0-A5 on Photon board to 6 terminal strip, same order.
//   Office Ceiling Switch was A0 Brown thermistat wire
//   Loft Switch           was A1 Red " "
//   Ramp Porch Switch     was A2 Yellow " "
//   Ramp Awning Switch    was A3 Green " "
//   Rear Porch Switch     was A4 Blue " "
//   Rear Awning Switch    was A5 White " "

// This must be run before either setup() or loop()
SYSTEM_THREAD(ENABLED);  // Allow running without internet
SYSTEM_MODE(SEMI_AUTOMATIC);

//TODO: convert to IPAddress
byte hueServer[4] = { 192, 168, 50, 21 };

IPAddress myAddress(192,168,50,15);
IPAddress netmask(255,255,255,0);
IPAddress gateway(192,168,50,1);
IPAddress dns(192,168,50,1);

IPAddress mqttAddress(192, 168, 50, 33);

bool officeMotion = false;
long lastOfficeMotion = 0;

bool officeDoor = false;
bool officeDoorCountdown = false;
long lastOfficeDoor = 0;

int watching = 0;
int cleaning = 0;
int partOfDay = 0;
int sleeping = 0;

void setup() {
    WiFi.selectAntenna(ANT_EXTERNAL);
    setWifiStaticIP();
    IoT::begin("192.168.50.33", "RearPanel");
    createDevices();
    handleDaylightSavings();
}

void setWifiStaticIP() {
    WiFi.setStaticIP(myAddress, netmask, gateway, dns);
    WiFi.useStaticIP();
}

void createDevices() {
    Device::add(new PartOfDay());

    // I2CIO4R4G5LE board
    // 4 Relays
    Device::add(new Curtain(I2CR4IO4, 0, "Curtain", "Office"));     // 2x Relays: 0, 1
    // Fading OfficeTrim results in door toggling, probably due to parallel wiring, so on/off only
    Device::add(new NCD4Relay(I2CR4IO4, 2, "OfficeTrim", "Office"));
    
    // 4 GPIO
    Device::add(new NCD4Switch(I2CR4IO4, 0, "OfficeDoor", "Office"));
    
    // Photon I/O
    Device::add(new PIR(A5, "OfficeMotion", "Office"));

    // I2CPWM8W80C board
    // 8 Dimmers
    Device::add(new NCD8Light(ADDRESS, 0, "OfficeCeiling", "Office", 2));
    Device::add(new NCD8Light(ADDRESS, 1, "Loft", "Office", 2));
    Device::add(new NCD8Light(ADDRESS, 2, "RampPorch", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 3, "RampAwning", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 4, "RearPorch", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 5, "RearAwning", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 6, "Piano", "Office", 2));
//    Device::add(new NCD8Light(ADDRESS, 7, "OfficeTrim", "Office", 0));

    // Activities/States - define for every other state.
    // Be careful to only define in 1 (this) controller.
    Device::add(new Device("sleeping", "All"));
    Device::add(new Device("cleaning", "All"));
    Device::add(new Device("watching", "All"));
    Device::add(new Device("RonHome", "All"));
    Device::add(new Device("ShelleyHome", "All"));
    
    // Checklist Items -  - define for every non-automated checklist item
    
    // Pre-Trip checklist items
    Device::add(new Device("startList", "All", 'X'));
    Device::add(new Device("checkTires", "All", 'X'));
    Device::add(new Device("dumpTanks", "All", 'X'));
    Device::add(new Device("fillWater", "All", 'X'));
    Device::add(new Device("fuel", "All", 'X'));
    Device::add(new Device("fillPropane", "All", 'X'));
    Device::add(new Device("checkRoof", "All", 'X'));
    Device::add(new Device("checkUnderRV", "All", 'X'));
    Device::add(new Device("planRoute", "All", 'X'));

    // Depart checklist items
    Device::add(new Device("bedSlideIn", "All", 'X'));
    Device::add(new Device("LRSlideIn", "All", 'X'));
    Device::add(new Device("rampAwningIn", "All", 'X'));
    Device::add(new Device("closeRamp", "All", 'X'));
    Device::add(new Device("rearAwningIn", "All", 'X'));
    Device::add(new Device("latchHandles", "All", 'X'));
    Device::add(new Device("frontAwningIn", "All", 'X'));
    Device::add(new Device("discPropane", "All", 'X'));
    Device::add(new Device("waterHeaterOff", "All", 'X'));
    Device::add(new Device("hitchTruck", "All", 'X'));
    Device::add(new Device("raiseLG", "All", 'X'));
    Device::add(new Device("discPower", "All", 'X'));

    // Arrive checklist items
    Device::add(new Device("connectPower", "All", 'X'));
    Device::add(new Device("lowerLG", "All", 'X'));
    Device::add(new Device("bedSlideOut", "All", 'X'));
    Device::add(new Device("LRSlidesOut", "All", 'X'));
    Device::add(new Device("openRamp", "All", 'X'));
    Device::add(new Device("rampAwningOut", "All", 'X'));
    Device::add(new Device("stepsDown", "All", 'X'));
    Device::add(new Device("rearAwningOut", "All", 'X'));
    Device::add(new Device("frontAwningOut", "All", 'X'));
    Device::add(new Device("propaneOn", "All", 'X'));
    Device::add(new Device("waterHose", "All", 'X'));
    Device::add(new Device("waterHeaterOn", "All", 'X'));
    Device::add(new Device("sewerHose", "All", 'X'));
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
    
    handleAutoGoodnight();
    handlePartOfDay();
    handleSleeping();
    handleOfficeMotion();
    handleOfficeDoor();
    handleWatching();
    handleCleaning();
}

/**
 * If 2:00 am and goodnight hasn't been issued
 * then set sleep = 1 anyways.
 */
void handleAutoGoodnight() {
    if(sleeping < ASLEEP && Time.hour() == 2) {
        IoT::mqttPublish("patriot/sleeping", "3");   // 3 = ASLEEP
        Device::setValue("sleeping", ASLEEP);
    }
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

/**
 * handleOfficeMotion
 *
 * Dependencies:
 *   int sleeping
 *   int partOfDay
 */
void handleOfficeMotion() {

    long loopTime = millis();
    int officeMotionChanged = Device::getChangedValue("OfficeMotion");
    
    if(officeMotionChanged > 0 ) {         // Motion?
        officeMotion = true;
        lastOfficeMotion = loopTime;

        //TODO: change this to only happen when dark
        Device::setValue("Piano", 50);
        
        if( partOfDay > SUNSET && sleeping > 0 && sleeping != ASLEEP) {
            if(Time.hour() > 4) {   // Motion after 5:00 is wakeup
                IoT::mqttPublish("patriot/sleeping", "1");   // AWAKE
                Device::setValue("sleeping", AWAKE);
            }
        }
        return;
    }
        
    // Timed shut-off
    if(officeMotion == true) {
        if(loopTime >= lastOfficeMotion+OFFICE_MOTION_TIMEOUT) {
            Log.info("Office motion timed out");
            officeMotion = false;
            //TODO: check other things like watching, sleeping, etc.
            Device::setValue("Piano", 0);
        }
    }
}

/**
 * handleOfficeDoor
 *
 * Dependencies:
 *   int sleeping
 *   int partOfDay
 */
void handleOfficeDoor() {

    // Timed shut-off after door closes
    long loopTime = millis();
    if(officeDoorCountdown == true) {
        if(loopTime >= lastOfficeDoor+OFFICE_DOOR_TIMEOUT) {
            Log.info("Office door timeout");
            officeDoorCountdown = false;
            //Turn off light if night and after sleeping
            if(partOfDay > SUNSET && sleeping == ASLEEP ) {
                Device::setValue("RearPorch", 0);
            }
        }
    }

    int officeDoorChanged = Device::getChangedValue("OfficeDoor");
    if( officeDoorChanged != -1) {
        if( officeDoorChanged > 0 ) {   // Door opened
            officeDoor = true;
            officeDoorCountdown = false;    // Reset it if it was in progress
            // If after sunset turn on porch light
            if( partOfDay > SUNSET ) {
                Device::setValue("RearPorch", 100);
            }
        } else {                        // Door closed
            officeDoor = false;
            lastOfficeDoor = millis();  // update timeout
            officeDoorCountdown = true;
            // Nothing else to do when door closes. Timer will shut off if needed.
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

        } else {
            watching = 0;
            //TODO: check if evening lights s/b on, etc.
            Log.info("watching did turn off");
        }
    }
}

/**
 * handleSleeping
 */
void handleCleaning() {
    int cleaningChanged = Device::getChangedValue("cleaning");
    if( cleaningChanged != -1 ) {
        if( cleaningChanged > 0 ) {
            cleaning = 100;
            Log.info("cleaning did turn on");
            //TODO: save current light state to restore when cleaning turned off
            setAllInsideLights( 100 );
        } else {
            cleaning = 0;
            //TODO: check if evening lights s/b on, etc.
            Log.info("cleaning did turn off");
            setAllInsideLights( 0 );
        }
    }
}


void setAllActivities(int value) {
    Device::setValue("cooking", value);
    Device::setValue("cleaning", value);
}

void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("officeceiling",70);
    Device::setValue("OfficeTrim", 100);
}

void setSunriseLights() {
    Log.info("setSunriseLights");
    setAllOutsideLights(0);
    //setAllInsideLights(0);
}

void setEveningLights() {
    Log.info("setEveningLights");
    Device::setValue("officeceiling",70);
    Device::setValue("piano", 100);
    Device::setValue("OfficeTrim", 100);
    setAllOutsideLights(100);
}

void setBedtimeLights() {
    Log.info("setBedtimeLights");
    setAllActivities(0);
    setAllInsideLights(0);
    Device::setValue("piano",70);
    setAllOutsideLights(0);
    Device::setValue("Curtain",0);
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    setAllActivities(0);
    setAllInsideLights(0);
    setAllOutsideLights(0);
    Device::setValue("Curtain",0);
}

void setWatchingLights(int level) {
    Log.info("setWatchingLights %d", level);
    // Nothing to do
}

void setAllInsideLights(int value) {
    Log.info("setAllInsideLights %d",value);
    Device::setValue("OfficeCeiling", value);
    Device::setValue("Loft", value);
    Device::setValue("Piano", value);
    Device::setValue("OfficeTrim", value);

}

void setAllOutsideLights(int value) {
    Log.info("setAllOutsideLights %d",value);
    Device::setValue("RampPorch", value);
    Device::setValue("RampAwning", value);
    Device::setValue("RearPorch", value);
    Device::setValue("RearAwning", value);
}
