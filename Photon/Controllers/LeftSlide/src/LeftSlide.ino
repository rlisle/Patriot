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
//#include <PatriotMR24.h>
#include <HueLight.h>
#include "secrets.h"   // Modify this to include your passwords: HUE_USERID

#define CONTROLLER_NAME "LeftSlide"
#define MQTT_BROKER "192.168.50.33"
#define CONNECT_TO_CLOUD true
#define LIVINGROOM_MOTION_TIMEOUT 3*60

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

#define MQTT_LOGGING true
//SerialLogHandler logHandler1(57600, LOG_LEVEL_ALL);

byte hueServer[4] = { 192, 168, 50, 39 };   // Changed 10/1/22

//bool couchPresenceFiltered = 0;
//long lastCouchPresence = 0;

//bool livingRoomMotion = false;
//long lastLivingRoomMotion = 0;

//int couchPresence = 0;

void setup() {
    WiFi.selectAntenna(ANT_EXTERNAL);
    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    createDevices();
}

void createDevices() {
    // Sensors
    Device::add(new PIR(A0, "LivingRoomMotion", "Living Room", LIVINGROOM_MOTION_TIMEOUT));
    //Device::add(new MR24(0, 0, "CouchPresence", "Living Room"));    // Was D3, D4

    // Philips Hue Lights (currently requires internet connection)
    Device::add(new HueLight("Bedroom", "Bedroom", "1", hueServer, HUE_USERID));
    Device::add(new HueLight("DeskLeft", "Office", "2", hueServer, HUE_USERID));
    Device::add(new HueLight("DeskRight", "Office", "3", hueServer, HUE_USERID));
    Device::add(new HueLight("Nook", "Living Room", "4", hueServer, HUE_USERID));

    // Lights
    Device::add(new Light(A7, "Couch", "Living Room", 2));
    Device::add(new Light(A5, "LeftVertical", "Living Room", 2));
}

/**
 * LOOP
 */
void loop() {
    IoT::loop();

    //TODO: move to MR24 plugin
//    handleCouchPresence();
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
