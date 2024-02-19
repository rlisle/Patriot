/**
Left Slide Controller
Description: This sketch controls the 2 lights and sensors in the left slide.
Author: Ron Lisle
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash left_slide2 --target 5.6.0" or shortcut "fls"

  New Photon 2 I/O Map
    A2/D13 LED1 PWM output
    A5/D14 LED2 PWM output
    S4/D19 PIR sensor
 
  Optional I/O
     2nd PIR S3/D18
 
  Previous I/O Map
    Tx HR24 sensor Rx
    Rx HR24 sensor Tx
    A0 PIR sensor
    A5 LED 2 PWM output
    A7 (WKP) LED1 PWM output
 
  Terminal Strip
    1 +3.3v (top)
    2 Gnd
    3 +12v
    4 -LED1
    5 -LED2
 
  Using SYSTEM_THREAD(ENABLED) is recommended,
  and runs network on separate theread.
  Using SYSTEM_MODE(SEMI_AUTOMATIC) we will
  manually connect, but everything is automatic
  after that. This allows running loop and MQTT
  even if no internet available
 
 History:
   8/6/23 Convert to Photon 2, remove Hue
   8/23/23 Embed plugins due to compiler issue
 */

#include <IoT.h>
//#include "math.h"
//#include <PatriotMR24.h>

#define CONTROLLER_NAME "LeftSlide"
#define MQTT_BROKER "192.168.0.33"
#define LIVINGROOM_MOTION_TIMEOUT 3*60

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

#define PIR_POLL_INTERVAL_MILLIS 500

// Generally uncomment only 1 of the following 2 lines
#define MQTT_LOGGING true
//SerialLogHandler logHandler1(57600, LOG_LEVEL_ALL);

// Timing
//msecs lastMinute = 0;
bool isTimingLivingRoomMotion;

//bool couchPresenceFiltered = 0;
//long lastCouchPresence = 0;
//int couchPresence = 0;

// Behaviors
#include "Behaviors.h"
#include "EventHandlers.h"

//-------------
// LOOP
//-------------
void loop() {
    IoT::loop();

    // //TODO: move into IoT
    // if(msecs()-60*1000 > lastMinute) {
    //     lastMinute = msecs();
    //     handleNextMinute();
    // }

//    handleCouchPresence();
}

void setup() {
//    WiFi.setCredentials(WIFI_SSID, WIFI_PASSWORD);
//    WiFi.selectAntenna(ANT_INTERNAL);
//    WiFi.useDynamicIP();

    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);

    // Behaviors
    setNextMinuteHandler(handleNextMinute);

    
    Device::setAnyChangedHandler(updateLights);
    Device::add(new Device("AnyoneHome", "Status", 'S'));
    Device::add(new Device("Cleaning", "Status", 'S'));
//    Device::add(new Device("Couch", "Status", 'S'));    // Need another name?
    Device::add(new Device("Nighttime", "Status", 'S'));
    Device::add(new Device("RonHome", "Status", 'S'));
    Device::add(new Device("ShelleyHome", "Status", 'S'));
    Device::add(new Device("Sleeping", "Status", 'S'));

    // Create Devices
    // Sensors
    Device::add(new PIR(D19, "LivingRoomMotion", "Living Room", LIVINGROOM_MOTION_TIMEOUT, handleLivingRoomMotion));
    //Device::add(new MR24(0, 0, "CouchPresence", "Living Room"));    // Was D3, D4

    // Lights (default 2s curve 2)
    Device::add(new Light(A2, "Couch", "Living Room"));         // Handles independenty
    Device::add(new Light(A5, "LeftVertical", "Living Room"));  // "
}

//TODO: move to MR24 plugin
/*void handleCouchPresence() {
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

//TODO: move to MR24 plugin
int filter(int sum, int value) {
    float flSum = float(sum);
    float flValue = float(value);
    // Super simple, but may be enough
    float result = (flSum * 0.5) + (flValue * 0.5);
    return (int)result;
}

//TODO: move to MR24 plugin
int quantize(int value) {
    if(value < 13) return 0;
    if(value < 38) return 25;
    if(value < 63) return 50;
    if(value < 88) return 75;
    return 100;
}
*/
