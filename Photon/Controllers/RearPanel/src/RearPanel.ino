/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle

 The controller is located in the office above the piano.
 
 Photon 2 pin assignments
 - No Photon 2 pins used directly.
 - Use I2C for all I/O.
 
 (old) Photon pin assignments
 - A2-A4 future 12v monitor ?
 - A5 PIR
 
 To update Photon:
   1. Edit this code
   2. Update IoT and plugins if needed
   3. Put Photon into safe mode using buttons (breathing magenta)
        Press both buttons, release reset, release setup when blinking magenta
   4. "particle flash RearPanel" or "frp"
 
 Since Cloud is not connected, photon should normally be breathing green

 Using SYSTEM_THREAD(ENABLED) is recommended,
 and runs network on separate theread.
 Using SYSTEM_MODE(AUTOMATIC)

 Migration from Photon 1 to Photon 2:
 1. Remove 12v power and disconnect current I2CR4IO4 board - note/photo wires
 2. Remove SCT board - note wires
 3. Install IoTR4IO4 with Photon 2 and adapter and USB power converter
 4. Reconnect wires to IoTR4IO4 board
 5. Reprogram/configure Address
 6. Reprogram FIR to use R4IO4
 
 Compiling: particle compile photon2 --target 5.4.1
 Flashing: particle flash rear_panel2 --target 5.4.1

 */

#include <IoT.h>
//#include <PatriotNCD4Switch.h>
//#include <PatriotNCD8Light.h>
//#include <PatriotCurtain.h>
//#include <PatriotNCD4Relay.h>
//#include <PatriotPIR.h>
//#include <PatriotNCD4PIR.h>
//#include <PatriotAwning.h>
#include "secrets.h"   // Modify this to include your passwords: HUE_USERID

#define CONTROLLER_NAME "RearPanel"
#define MQTT_BROKER "192.168.50.33"
#define MQTT_LOGGING true

#define OFFICE_MOTION_TIMEOUT 15

#define ADDRESS 1      // NCD8Light PWM board address A0 jumper set
#define I2CR4IO4 0x20  // 4xRelay+4GPIO address (0x20 = no jumpers)

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

bool officeMotion = false;
long lastOfficeMotion = 0;

bool officeDoor = false;
bool officeDoorCountdown = false;
long lastOfficeDoor = 0;

void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);
    
    //WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    createDevices();
    
//    setupNCD4Switch();  // Debugging NCD4Switch
}

//long    lastPollTime = 0;
//int     filter = 0;
//int8_t  boardAddress = 0x20;
//int8_t  switchBitmap = 0x10;    // Hardcode to 1st input
//
//void setupNCD4Switch() {
//    byte status;
//    int  retries;
//
//    // Only the first device on the I2C link needs to enable it
//    if(!Wire.isEnabled()) {
//        Wire.begin();
//    }
//
//    retries = 0;
//    do {
//        Wire.beginTransmission(boardAddress);
//        Wire.write(0x00);                   // Select IO Direction register
//        Wire.write(0xf0);                   // 0-3 relays, 4-7 in
//        status = Wire.endTransmission();    // Write 'em, Dano
//    } while( status != 0 && retries++ < 3);
//    if(status != 0) {
//        Log.error("Set IODIR failed");
//    }
//    
//    retries = 0;
//    do {
//        Wire.beginTransmission(boardAddress);
//        Wire.write(0x06);                   // Select pull-up resistor register
//        Wire.write(0xf0);                   // pull-ups enabled on inputs
//        status = Wire.endTransmission();
//    } while( status != 0 && retries++ < 3);
//    if(status != 0) {
//        Log.error("Set GPPU failed");
//    }
//}

/**
 NCD4Switch Defines
 */
#define POLL_INTERVAL_MILLIS 100
#define FILTER_INCREMENT 25

class NCD4Switch : public Device
{
 private:
    long    _lastPollTime;
    int8_t  _filter;

    int8_t  _boardAddress;
    int8_t  _switchBitmap;
    
    bool      isSwitchOn();
    bool      isTimeToCheckSwitch();
    bool      didSwitchChange();
    void      notify();

 public:
    NCD4Switch(int8_t boardAddress, int8_t switchIndex, String name, String room);
    
    void    begin();
    void    reset();
    void    loop();
};


void createDevices() {
    // I2CIO4R4G5LE board
    // 4 Relays
//    Device::add(new Curtain(I2CR4IO4, 0, "Curtain", "Office"));     // 2x Relays: 0, 1
    // Device::add(new Awning(I2CR4IO4, 2, "RearAwning", "Outside")); // 2x Relays: 2, 3
    
    // 4 GPIO
    Device::add(new NCD4Switch(I2CR4IO4, 0, "OfficeDoor", "Office"));
    //Device::add(new NCD4PIR(I2CR4IO4, 1, "OfficeMotion", "Office", OFFICE_MOTION_TIMEOUT));

    // (deprecated) Photon I/O
    //Device::add(new PIR(A5, "OfficeMotion", "Office", OFFICE_MOTION_TIMEOUT));

    // I2CPWM8W80C board
    // 8 Dimmers
//    Device::add(new NCD8Light(ADDRESS, 0, "OfficeCeiling", "Office", 2));
//    Device::add(new NCD8Light(ADDRESS, 1, "Loft", "Office", 2));
//    Device::add(new NCD8Light(ADDRESS, 2, "RampPorch", "Outside", 2));
//    Device::add(new NCD8Light(ADDRESS, 3, "RampAwning", "Outside", 2));
//    Device::add(new NCD8Light(ADDRESS, 4, "RearPorch", "Outside", 2));
//    Device::add(new NCD8Light(ADDRESS, 5, "RearAwning", "Outside", 2));
//    Device::add(new NCD8Light(ADDRESS, 6, "Piano", "Office", 2));
//    //Device::add(new NCD8Light(ADDRESS, 7, "Unused", "Office", 2));
//
    // Pseudo Devices
    Device::add(new Device("AnyoneHome", "All", 'X'));
    Device::add(new Device("RonHome", "All", 'X'));
    Device::add(new Device("ShelleyHome", "All", 'X'));
    Device::add(new Device("Nighttime", "All", 'X'));

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

/**
 * LOOP
 */
void loop() {
    IoT::loop();
}

/**
 NCD4Switch
 */
/**
 * Constructor
 * @param boardAddress is the board address set by jumpers (0-7) 0x20-0x27
 * @param switchIndex is the switch number on the NCD board (0-3)
 * @param name String name used to address the relay.
 */
NCD4Switch::NCD4Switch(int8_t boardAddress, int8_t switchIndex, String name, String room)
    : Device(name, room)
{
    _boardAddress = boardAddress;   // 0x20 (no jumpers)
    _lastPollTime = 0;
    _type         = 'S';
    _filter       = 0;
    
    if(switchIndex > 0 && switchIndex <= 3) {
        _switchBitmap = 0x10 << switchIndex;
    } else {
        _switchBitmap = 0x10;   // If 0 or invalid, set to first switch
    }
}

void NCD4Switch::begin() {

    byte status;
    int  retries;

    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(0xf0);                   // 0-3 relays, 4-7 in
        status = Wire.endTransmission();    // Write 'em, Dano
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set IODIR failed");
    }
    
    retries = 0;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x06);                   // Select pull-up resistor register
        Wire.write(0xf0);                   // pull-ups enabled on inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set GPPU failed");
    }
}

/**
 * isSwitchOn
 * Return state of switch (inverted: low = 100, high = 0)
 */
bool NCD4Switch::isSwitchOn() {
    int retries = 0;
    int status;
    do {
        Wire.beginTransmission(_boardAddress);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Error selecting GPIO register");
    }
    
    Wire.requestFrom(_boardAddress, 1);      // Read 1 byte
    
    if (Wire.available() == 1)
    {
        int data = Wire.read();
        return((data & _switchBitmap) == 0);    // Inverted
    }
    Log.error("Error reading switch");
    return false;
}

void NCD4Switch::reset() {
    Log.error("Resetting board");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

    // Issue PCA9634 SWRST
    Wire.beginTransmission(_boardAddress);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("NCD4Switch reset write failed for switch bitmap: "+String(_switchBitmap)+", re-initializing board");
    }
    begin();
}



/**
 * loop()
 */
void NCD4Switch::loop()
{
    if (isTimeToCheckSwitch())
    {
        if (didSwitchChange())
        {
            notify();
        }
    }
}

// Private Helper Methods
/**
 * isTimeToCheckSwitch()
 * @return bool if enough time has elapsed to sample switch again
 */
bool NCD4Switch::isTimeToCheckSwitch()
{
    long currentTime = millis();
    if (currentTime < _lastPollTime + POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}


/**
 * didSwitchChange()
 * @return bool if switch has changed since last reading
 */
bool NCD4Switch::didSwitchChange()
{
    int newValue = isSwitchOn() ? 100 : 0;
    bool oldState = (_value != 0);
    
    if(newValue == 100) {   // Is switch on?
        _filter += FILTER_INCREMENT;
        if(_filter > 100) {
            _filter = 100;
        }
    } else {    // Switch is off
        _filter -= FILTER_INCREMENT;
        if(_filter < 0) {
            _filter = 0;
        }
    }

    if(oldState == false && _filter == 100) {
        _value = 100;
        return true;
    }
    
    if(oldState == true && _filter == 0) {
        _value = 0;
        return true;
    }
    
    return false;
}


/**
 * notify()
 * Publish switch state
 */
void NCD4Switch::notify()
{
    String message = String(_value);
    IoT::publishMQTT(_name + "/get/position",message);
}


