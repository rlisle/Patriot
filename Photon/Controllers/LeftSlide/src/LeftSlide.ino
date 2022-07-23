/**
Left Slide Controller
Description: This sketch controls the 2 lights and sensors in the left slide.
Author: Ron Lisle
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash LeftSlide"

  I/O Map
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
 
  Using SYSTEM_THREAD(ENABLED) is recommended,
  and runs network on separate theread.
  Using SYSTEM_MODE(SEMI_AUTOMATIC) we will
  manually connect, but everything is automatic
  after that. This allows running loop and MQTT
  even if no internet available
 */

#include <IoT.h>
#include <PatriotLight.h>
#include <PatriotPIR.h>
#include <PatriotMR24.h>
#include <HueLight.h>
#include "secrets.h"   // Modify this to include your passwords: HUE_USERID

#define LIVINGROOM_MOTION_TIMEOUT 15*1000

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

//TODO: convert to IPAddress
byte hueServer[4] = { 192, 168, 50, 21 };

IPAddress myAddress(192,168,50,30);
IPAddress netmask(255,255,255,0);
IPAddress gateway(192,168,50,1);
IPAddress dns(192,168,50,1);

IPAddress mqttAddress(192, 168, 50, 33);


bool couchPresenceFiltered = 0;
long lastCouchPresence = 0;

bool livingRoomMotion = false;

int watching = 0;
int cleaning = 0;
int couchPresence = 0;

// Move to IoT eventually
int partOfDay = 0;
int sleeping = 0;


void setup() {
    WiFi.selectAntenna(ANT_EXTERNAL);
    setWifiStaticIP();
    IoT::begin("192.168.50.33","LeftSlide");
    createDevices();
}

void setWifiStaticIP() {
    WiFi.setStaticIP(myAddress, netmask, gateway, dns);
    WiFi.useStaticIP();
}

void createDevices() {
    // Sensors
    Device::add(new PIR(A0, "LivingRoomMotion", "Living Room", LIVINGROOM_MOTION_TIMEOUT));
    Device::add(new MR24(0, 0, "CouchPresence", "Living Room"));    // Was D3, D4

    // Philips Hue Lights (currently requires internet connection)
    Device::add(new HueLight("Bedroom", "Bedroom", "1", hueServer, HUE_USERID));
    Device::add(new HueLight("DeskLeft", "Office", "2", hueServer, HUE_USERID));
    Device::add(new HueLight("DeskRight", "Office", "3", hueServer, HUE_USERID));
    Device::add(new HueLight("Nook", "Living Room", "4", hueServer, HUE_USERID));

    // Lights
    Device::add(new Light(A7, "Couch", "Living Room", 2));
    Device::add(new Light(A5, "LeftVertical", "Living Room", 2));
        
    // Activities/States
    Device::add(new Device("cleaning", "All"));
    Device::add(new Device("watching", "All"));
    
    Device::add(new Device("desk", "Office")); // Groups both desk lamps together
    
    //Move to IoT eventually
    Device::add(new Device("partofday", "All"));
    Device::add(new Device("sleeping", "All"));
}

void loop() {
    IoT::loop();

    handlePartOfDay();
    handleSleeping();
    handleLivingRoomMotion();
    handleWatching();
    handleCleaning();
    handleCouchPresence();
    handleDesk();
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
//            if(partOfDay > SUNSET || (partOfDay==0 && Time.hour() < 8)) {
//                Log.info("It is morning");
                setMorningLights();
//            }
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
 * handleLivingRoomMotion
 *
 * Dependencies
 *   int partOfDay
 *   int sleeping
 */
void handleLivingRoomMotion() {

    int livingRoomMotionChanged = Device::getChangedValue("LivingRoomMotion");
    if(livingRoomMotionChanged == 100) {
        Log.info("LivingRoom Motion detected");
        Device::setValue("LeftVertical", 50);

        // Determine if this is Ron getting up
        if( partOfDay > SUNSET && sleeping != AWAKE) {
            if(Time.hour() > 3 && Time.hour() < 9) {   // Motion after 4:00 is wakeup
                IoT::mqttPublish("patriot/sleeping", "1");   // AWAKE
                Device::setValue("sleeping", AWAKE);
            }
        }
        livingRoomMotion = true;

    } else if(livingRoomMotionChanged == 0) {
        Log.info("LivingRoom Motion stopped");
        Device::setValue("LeftVertical", 0);
        livingRoomMotion = false;

    } // Ignore -1
    
    return;
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
            Device::setValue("Nook", 100);

        } else {
            watching = 0;
            //TODO: check if evening lights s/b on, etc.
            Log.info("watching did turn off");
            Device::setValue("Couch", 0);
            Device::setValue("Nook", 0);
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

void handleDesk() {
    int deskChanged = Device::getChangedValue("desk");
    if( deskChanged != -1 ) {
        Log.info("desk changed");
        setDeskLamps( deskChanged );
    }
}

void handleCouchPresence() {
    int couchPresenceChanged = Device::getChangedValue("CouchPresence");
    if( couchPresenceChanged != -1) {
        
        couchPresenceFiltered = filter(couchPresenceFiltered,couchPresenceChanged);
        int newCouchPresence = quantize(couchPresenceFiltered);
        
        if(newCouchPresence != couchPresence) {
            couchPresence = newCouchPresence;
            switch(couchPresence){
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
        }
        Log.info("Couch presence = %d",couchPresence);
    }
}

int filter(int sum, int value) {
    float flSum = float(sum);
    float flValue = float(value);
    // Super simple, but may be enough
    float result = (flSum * 0.5) + (flValue * 0.5);
    return (int)result;
}

int quantize(int value) {
    if(value < 13) return 0;
    if(value < 38) return 25;
    if(value < 63) return 50;
    if(value < 88) return 75;
    return 100;
}

void setAllActivities(int value) {
    Device::setValue("cleaning", value);
    Device::setValue("watching", value);
}

void setAllLights(int value) {
    Log.info("setAllLights %d",value);
    Device::setValue("Couch", value);
    Device::setValue("LeftVertical", value);
    Device::setValue("DeskLeft",value);
    Device::setValue("DeskRight",value);
    Device::setValue("Nook",value);
}

void setDeskLamps(int value) {
    Log.info("setDeskLamps");
    Device::setValue("DeskLeft",value);
    Device::setValue("DeskRight",value);
}

void setMorningLights() {
    Log.info("setMorningLights");
    Device::setValue("LeftVertical", 30);
    Device::setValue("DeskLeft",100);
    Device::setValue("DeskRight",100);
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
    Device::setValue("Bedroom", 100);   // Turn on bedroom lamp
}

void setSleepingLights() {
    Log.info("setSleepingLights");
    setAllActivities(0);
    setAllLights(0);
    Device::setValue("Bedroom",0);
}
