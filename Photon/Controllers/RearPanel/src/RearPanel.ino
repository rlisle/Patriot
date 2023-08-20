/**
RearPanel Controller
Description: This sketch controls all the switches in the Cyclone 4005 rear control panel.
Author: Ron Lisle

 The controller is located in the office above the piano.
 
 Updated to Photon 2
 - No Photon 2 pins used directly.
 - Use I2C for all I/O.
 
 To update code:
   1. Edit this code
   2. Update IoT if needed
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
#include "secrets.h"   // Modify this to include your passwords: HUE_USERID
#include "math.h"

#define MILLIS_PER_SECOND 1000
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
int8_t pca9634address;

// MARK: - Setup
void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);
    
    //WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    
    //Consolidate PCA9634 initialization
    initializePCA9634(I2CR4IO4, 0xf0);   // Address 0x20 (no jumpers), all 4 GPIOs inputs
    createDevices();
}

// MARK: - NCD4Switch
#define POLL_INTERVAL_MILLIS 100
#define FILTER_INCREMENT 25
class NCD4Switch : public Device {
 private:
    long    _lastPollTime;
    int8_t  _filter;
    int8_t  _switchBitmap;
    
    bool      isSwitchOn();
    bool      isTimeToCheckSwitch();
    bool      didSwitchChange();
    void      notify();

 public:
    NCD4Switch(int8_t switchIndex, String name, String room);
    
    void    begin();
    void    reset();
    void    loop();
};
NCD4Switch::NCD4Switch(int8_t switchIndex, String name, String room)
    : Device(name, room) {
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

    // initializePCA9634 does all this once instead of for every device
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
//        Wire.beginTransmission(pca9634address);
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
//        Wire.beginTransmission(pca9634address);
//        Wire.write(0x06);                   // Select pull-up resistor register
//        Wire.write(0xf0);                   // pull-ups enabled on inputs
//        status = Wire.endTransmission();
//    } while( status != 0 && retries++ < 3);
//    if(status != 0) {
//        Log.error("Set GPPU failed");
//    }
}
bool NCD4Switch::isSwitchOn() {
    int retries = 0;
    int status;
    do {
        Wire.beginTransmission(pca9634address);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Error selecting GPIO register");
    }
    
    Wire.requestFrom(pca9634address, 1);      // Read 1 byte
    
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
    Wire.beginTransmission(pca9634address);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("NCD4Switch reset write failed for switch bitmap: "+String(_switchBitmap)+", re-initializing board");
    }
    begin();
}
void NCD4Switch::loop() {
    if (isTimeToCheckSwitch())
    {
        if (didSwitchChange())
        {
            notify();
        }
    }
}
bool NCD4Switch::isTimeToCheckSwitch() {
    // return if enough time has elapsed to sample switch again
    long currentTime = millis();
    if (currentTime < _lastPollTime + POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}
bool NCD4Switch::didSwitchChange() {
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
void NCD4Switch::notify() {
    String message = String(_value);
    IoT::publishMQTT(_name + "/get/position",message);
}

// MARK: - Curtain
#define FULL_TIME_MILLIS 6000   // Duration of full open to close or vis-vis
#define MILLIS_PER_PERCENT 60   // FULL_TIME_MILLIS / 100
#define MILLIS_PER_UPDATE 1000  // Send updates every second
#define PULSE_MILLIS 100
// Mode (relay bit)
#define OPEN_CURTAIN 2
#define CLOSE_CURTAIN 1
class Curtain : public Device {
 private:
    unsigned long _stopMillis;    // time to change start/stop pulse
    unsigned long _updateMillis;  // time to send next update msg
    unsigned long _startMillis;

    int8_t  _relayIndex;
    
    int8_t  _mode;  // 1 = close, 2 = open, 0 = idle
    int8_t  _stage; //1 = start pulse, 2=running, 3=stop pulse, 4=notDone
    bool    _holding;
    
    int8_t  _startPosition;

    void    pulse(bool start);
    bool    isCurtainRunning();
    bool    isTimeToChangePulse();
    int     readCurrentState();
    int     currentPosition();
    
 public:
    Curtain(int8_t relayIndex, String name, String room);
    
    void    begin();
    void    reset();
    void    setValue(int percent);  // Target state
    void    setHold(bool holding);
    void    loop();
};
Curtain::Curtain(int8_t relayIndex, String name, String room) : Device(name, room) {
    _relayIndex  = relayIndex;      // 0 (first 2 relays)
    _stopMillis = 0;
    _mode = 0;
    _stage = 0;
    _type  = 'C';
    _holding = false;
}
void Curtain::begin() {

    // Handled in pcr9634init() now.
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
//        Wire.beginTransmission(pca9634address);
//        Wire.write(0x00);                 // Select IO Direction register
//        Wire.write(0xf0);                 // 0-3 out, 4-7 in
//        status = Wire.endTransmission();  // Write 'em, Dano
//    } while( status != 0 && retries++ < 3);
//    if(status != 0) {
//        Log.error("Set IODIR failed");
//    }
//
//    retries = 0;
//    do {
//        Wire.beginTransmission(pca9634address);
//        Wire.write(0x06);        // Select pull-up resistor register
//        Wire.write(0xf0);        // pull-ups enabled on inputs
//        status = Wire.endTransmission();
//    } while( status != 0 && retries++ < 3);
//    if(status != 0) {
//        Log.error("Set GPPU failed");
//    }
}
void Curtain::setValue(int percent) {
    // This is how things are turned on or off in Patriot
    // 0 = closed, 100 = open
    //FOR DEBUGGING, SETTING ONLY OPEN OR CLOSED

    if(percent == _value) {
        Log.warn("Curtain setValue is the same as previous value, ignoring");
        //TODO: do we want to issue open/close again just in case?
        return;
    }

    _startPosition = _value;
    _startMillis = millis();
    
    _value = percent;           // Should this report current instead?
    _holding = false;
    _updateMillis = millis() + MILLIS_PER_UPDATE;
    Log.info("_updateMillis = %ld",_updateMillis);

    // Send HomeKit acknowledgement
    IoT::publishMQTT("/ack/" + _name + "/set",String(percent));
    
    // Send position updates
    IoT::publishMQTT(_name + "/get",String(percent));
    IoT::publishMQTT(_name + "/position",String(_startPosition));

    if(_value > _startPosition) {
        _mode = OPEN_CURTAIN;
        IoT::publishMQTT(_name + "/state", "increasing");

    } else {
        _mode = CLOSE_CURTAIN;
        IoT::publishMQTT(_name + "/state", "decreasing");
    }

    // We only need a single pulse if opening or closing all the way
    if(_value == 0 || _value == 100) {
        _stage = 3;
    } else {
        _stage = 1;
    }
    _stopMillis = millis() + PULSE_MILLIS;
    pulse(true);
}
void Curtain::setHold(bool holding) {
    if(holding == true) {
        if(_holding == true) {  // Already holding?
            return;
        }
        //TODO: stop movement, but remember target in case hold false
        
    } else {        // resume
        if(_holding == false) { // Not currently holding?
            return;
        }
        //TODO: resume movement - same as setValue using old target
        
    }
    _holding = holding;
    Log.warn("Curtain setHold not implemented");
}
void Curtain::pulse(bool high) {
    
    Log.info("Curtain pulse %s",high ? "high" : "low");

    int currentState = readCurrentState();
    
    byte bitmap = 0x01 << (_relayIndex + _mode - 1);
    if(high) {
        currentState |= bitmap;    // Set relay's bit
    } else {
        bitmap = 0xff ^ bitmap;
        currentState &= bitmap;
    }

    byte status;
    int retries = 0;
    do {
        Wire.beginTransmission(pca9634address);
        Wire.write(0x09);
        Wire.write(currentState);
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);

    if(status != 0) {
        Log.error("Error pulsing relay %d %s", bitmap, high ? "high" : "low");
    }
}
void Curtain::loop() {
    if(isCurtainRunning()) {
        
        if(isTimeToChangePulse()) {
            switch(_stage) {
                case 1:
                    Log.info("Curtain end-of-start pulse");
                    pulse(false);
                    
                    _stopMillis = millis() + ((FULL_TIME_MILLIS *  abs(_startPosition - _value)) / 100) - PULSE_MILLIS;
                    
                    _stage = 2;
                    break;
                case 2:
                    Log.info("Curtain start-of-end pulse");
                    pulse(true);
                    _stopMillis = millis() + PULSE_MILLIS;
                    _stage = 3;
                    break;
                case 3:
                    Log.info("Curtain end-of-end pulse");
                    pulse(false);
                    _stopMillis = millis() + FULL_TIME_MILLIS - PULSE_MILLIS;
                    _stage = 4;
                    break;
                case 4:
                    _stage = 0;
                    IoT::publishMQTT(_name + "/state", "stopped");
                    break;
                default:
                    Log.error("Invalid _stage %d",_stage);
            }
            
        }
        
        // Calculate periodic HomeKit updates to getCurrentPosition
        if(millis() >= _updateMillis) {
            _updateMillis += MILLIS_PER_UPDATE;
            
            int percentDelta = (int)((millis() - _startMillis) / MILLIS_PER_PERCENT);
            if(_mode == CLOSE_CURTAIN) percentDelta = -percentDelta;
//            Log.info("DBG: _startPosition = %d",_startPosition);
//            Log.info("DBG: curtain percent delta = %d",percentDelta);
//            Log.info("DBG: _startMillis = %ld",_startMillis);
//            Log.info("DBG: millis = %ld",millis());
//            Log.info("DBG: millis delta = %ld",millis() - _startMillis);
//            _value = _startPosition + percentDelta;
//            IoT::publishMQTT(_name + "/position", String(_value));
            IoT::publishMQTT(_name + "/position", String(_startPosition + percentDelta));
        }
    }
};
bool Curtain::isCurtainRunning() {
    return(_stage != 0);
}
bool Curtain::isTimeToChangePulse() {
    return(millis() >= _stopMillis);
}
void Curtain::reset() {
    Log.error("Resetting board");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

    // Issue PCA9634 SWRST
    Wire.beginTransmission(pca9634address);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("Curtain reset write failed for relayIndex: "+String(_relayIndex)+", re-initializing board");
    }
    begin();
}
int Curtain::readCurrentState() {
    int retries = 0;
    int status;
    do {
        Wire.beginTransmission(pca9634address);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Curtain: Error selecting GPIO register");
    }
    
    Wire.requestFrom(pca9634address, 1);      // Read 1 byte
    
    if (Wire.available() != 1)
    {
        Log.error("Curtain: Error reading current state");
        return 0;
    }
    
    int data = Wire.read();
    return(data);
}

// MARK: - NCD4PIR
#define PIR_POLL_INTERVAL_MILLIS 1000
class NCD4PIR : public Device {
 private:
    long    _lastPollTime;
    long    _lastMotion;
    long    _timeoutMSecs;
    int8_t  _gpioBitmap;

    bool    isSensorOn();
    bool    isTimeToCheckSensor();
    bool    didSensorChange();
    void    notify();

 public:
    NCD4PIR(int8_t gpioIndex, String name, String room, int timeoutSecs);
    
    void    begin();
    void    reset();
    void    loop();
    // Override to prevent MQTT from setting _percent.
    void setValue(int percent) { return; };
};
NCD4PIR::NCD4PIR(int8_t gpioIndex, String name, String room, int timeoutSecs) : Device(name, room), _timeoutMSecs(timeoutSecs * 1000) {
    _lastPollTime = 0;
    _lastMotion   = 0;
    _type         = 'M';    // Motion
    
    if(gpioIndex > 0 && gpioIndex <= 3) {
        _gpioBitmap = 0x10 << gpioIndex;
    } else {
        _gpioBitmap = 0x10;   // If 0 or invalid, set to first gpio
    }
}
void NCD4PIR::begin() {
    // Nothing to do here.
    // initializePCA9634 does all this once instead of for every device
}
bool NCD4PIR::isSensorOn() {
    return pca9634isInputOn(_gpioBitmap);
}
void NCD4PIR::reset() {
    Log.error("Resetting board");
    resetPCA9634();
}
void NCD4PIR::loop() {
    if (isTimeToCheckSensor())
    {
        if (didSensorChange())
        {
            notify();
        }
    }
}
bool NCD4PIR::isTimeToCheckSensor() {
    long currentTime = millis();
    if (currentTime < _lastPollTime + PIR_POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}
bool NCD4PIR::didSensorChange() {
    //We shouldn't need to filter or debounce
    int newValue = isSensorOn() ? 100 : 0;
    if(newValue != _value) {
        _value = newValue;
        return true;
    }
    return false;
}
void NCD4PIR::notify() {
    String message = String(_value);
    IoT::publishMQTT(_name + "/get/position",message);
}

// MARK: - NCD8Light
class NCD8Light : public Device {
 private:
    int8_t  _lightNum;                 // Up to 8 lights supported
    int8_t   _address;                 // Address of board (eg. 0x40)

    int      _dimmingDuration;
    float    _currentLevel;            // Use for smooth dimming transitions.
    float    _targetLevel;
    float    _incrementPerMillisecond;
    
    long     _lastUpdateTime;

    int8_t  initializeBoard();
    void    outputPWM();
    void    startSmoothDimming();
    int     scalePWM(float value);

 public:
    NCD8Light(int8_t address, int8_t lightNum, String name, String room, int8_t duration = 0);
    void    begin();
    void    reset();
    void    setValue(int value);
    void    loop();
};
NCD8Light::NCD8Light(int8_t address, int8_t lightNum, String name, String room, int8_t duration) : Device(name, room) {
    /**
     * @param address is the board address set by jumpers (0-7) 0x01 if low switch set, 0x40 if high
     * @param lightNum is the channel number on the NCD 8 Light board (0-7)
     * @param name String name used to address the light.
     * @param duration Optional seconds value to transition. 0 = immediate, no transition.
     */
    _address = address;
    _lightNum   = lightNum;
    _dimmingDuration = duration;
    _currentLevel = 0.0;
    _targetLevel = 0.0;
    _incrementPerMillisecond = 0.0;
    _lastUpdateTime = 0;
    _type = 'L';
}
void NCD8Light::begin() {
    initializeBoard();
}
int8_t NCD8Light::initializeBoard() {
    int status;

    // Only the first light loaded needs to initialize the I2C link
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    Wire.beginTransmission(_address);   // Seems unnecessary
    status = Wire.endTransmission();

    if(status == 0) {
        Wire.beginTransmission(_address);
        Wire.write(0);          // Control register - No AI, point to reg0 Mode1
        Wire.write(0);          // Mode1 reg. Osc on, disable AI, subaddrs, allcall
        Wire.endTransmission();

        Wire.beginTransmission(_address);
        Wire.write(1);          // Mode2 register
        Wire.write(0x04);       // Dimming, Not inverted, totem-pole
        Wire.endTransmission();

        Wire.beginTransmission(_address);
        Wire.write(0x8c);       // AI + LEDOUT0
        Wire.write(0xaa);       // LEDOUT0 LEDs 0-3 dimming
        Wire.write(0xaa);       // LEDOUT1 LEDS 4-7 dimming
        Wire.endTransmission();

        outputPWM();            // Force light off
        
        Log.info("InitializeBoard " + _name + " sucess");
        
    } else {
        Log.error("InitializeBoard " + _name + " FAILED!");
    }

    return status;
}
void NCD8Light::reset() {
    Log.error("Resetting board");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

    // Issue PCA9634 SWRST
    Wire.beginTransmission(_address);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("NCD8Light reset write failed for light "+String(_lightNum)+", reseting Wire");
    }
    initializeBoard();
}
void NCD8Light::setValue(int value) { // value = 0-100 (percent)
    if( value == _value ) {
        Log.info("Dimmer " + _name + " setValue " + String(value) + " same so outputPWM without dimming");
        _currentLevel = _value;
        outputPWM();
        return;
    }
    
    _currentLevel = _value;   // hold previous value for transitioning
    _value = value;
    _targetLevel = value;
    if(_dimmingDuration == 0) {
        _currentLevel = _targetLevel;
        outputPWM();

    } else {
        startSmoothDimming();
    }
}
void NCD8Light::startSmoothDimming() {
    if((int)_currentLevel != _targetLevel){
        _lastUpdateTime = millis();
        float delta = _targetLevel - _currentLevel;
        _incrementPerMillisecond = delta / (float(_dimmingDuration) * 1000);
        Log.info("Light "+_name+" setting increment to "+String(_incrementPerMillisecond));
    }
}
void NCD8Light::loop() {
    // Is fading transition underway?
    if(_currentLevel == _targetLevel) {
        return;
    }
    
    // _currentLevel, _targetLevel, and _incrementPerMillisend are floats for smoother transitioning
    
    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentLevel += _incrementPerMillisecond * millisSinceLastUpdate;
    if(_incrementPerMillisecond > 0.0) {
        if(_currentLevel > _targetLevel) {
            _currentLevel = _targetLevel;
        }
    } else {
        if(_currentLevel < _targetLevel) {
            _currentLevel = _targetLevel;
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM();
};
void NCD8Light::outputPWM() {
    int reg = 2 + _lightNum;
    
    int retryCount = 3;
    byte status;
    do {
        Wire.beginTransmission(_address);
        Wire.write(reg);
        Wire.write(scalePWM(_currentLevel));
        status = Wire.endTransmission();
        retryCount--;
    } while(status != 0 && retryCount > 0);
    
    if(status != 0){
        reset();
        retryCount = 5;
        do {
            Wire.beginTransmission(_address);
            Wire.write(reg);
            Wire.write(scalePWM(_currentLevel));
            status = Wire.endTransmission();
            retryCount--;
        } while(status != 0 && retryCount > 0);
        
        if(status != 0) {
            Log.error("NCD8Light outputPWM write failed twice for light "+String(_lightNum)+", level = "+String(_currentLevel));
            reset();
        }
    }
}
int NCD8Light::scalePWM(float value) { //Convert 0-100 to 0-255 scaled
    if (value <= 0) return 0;
    if (value >= 100) return 255;

    //TODO: This is too extreme. Need to refine algorithm
    float base = 1.05697667;
    float pwm = pow(base,value);
    if (pwm > 255) {
        pwm = 255;
    }
    return (int)pwm;
}

// MARK: - RearPanel
void initializePCA9634(int address, int iomap) {
    byte status;
    int  retries;
    pca9634address = address; // 0x20 = no jumpers
    
    // Only the first device on the I2C link needs to enable it
    if(!Wire.isEnabled()) {
        Wire.begin();
    }

    retries = 0;
    do {
        Wire.beginTransmission(pca9634address);
        Wire.write(0x00);                   // Select IO Direction register
        Wire.write(iomap);                   // 0-3 relays, 4-7 inputs
        status = Wire.endTransmission();    // Write 'em, Dano
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set IODIR failed");
        return;
    }
    
    retries = 0;
    do {
        Wire.beginTransmission(pca9634address);
        Wire.write(0x06);                   // Select pull-up resistor register
        Wire.write(0xf0 & iomap);           // pull-ups enabled on all inputs
        status = Wire.endTransmission();
    } while( status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Set GPPU failed");
    }
}
void resetPCA9634() {
    Log.error("Resetting PCA9634");
    Wire.reset();
    // Do we need any delay here?
    Wire.begin();

    // Issue PCA9634 SWRST
    Wire.beginTransmission(pca9634address);
    Wire.write(0x06);
    Wire.write(0xa5);
    Wire.write(0x5a);
    byte status = Wire.endTransmission();
    if(status != 0){
        Log.error("resetPCA9634 reset write failed");
    }
    initializePCA9634(I2CR4IO4, 0xf0);
}

bool pca9634isInputOn(int bitmap) {
    int retries = 0;
    int status;
    do {
        Wire.beginTransmission(pca9634address);
        Wire.write(0x09);       // GPIO Register
        status = Wire.endTransmission();
    } while(status != 0 && retries++ < 3);
    if(status != 0) {
        Log.error("Error selecting GPIO register");
        return false;
    }
    
    Wire.requestFrom(pca9634address, 1);      // Read 1 byte
    
    if (Wire.available() == 1)
    {
        int data = Wire.read();
        return((data & bitmap) == 0);    // Inverted
    }
    Log.error("Error reading switch");
    return false;
}

void createDevices() {
    // I2CIO4R4G5LE board
    // 4 Relays
    Device::add(new Curtain(0, "Curtain", "Office"));     // 2x Relays: 0, 1
    // Device::add(new Awning(2, "RearAwning", "Outside")); // 2x Relays: 2, 3
    
    // 4 GPIO
    Device::add(new NCD4Switch(0, "OfficeDoor", "Office"));
    Device::add(new NCD4PIR(1, "OfficeMotion", "Office", OFFICE_MOTION_TIMEOUT));

    // (deprecated) Photon I/O
    //Device::add(new PIR(A5, "OfficeMotion", "Office", OFFICE_MOTION_TIMEOUT));

    // I2CPWM8W80C board
    // 8 Dimmers
    Device::add(new NCD8Light(ADDRESS, 0, "OfficeCeiling", "Office", 2));
    Device::add(new NCD8Light(ADDRESS, 1, "Loft", "Office", 2));
    Device::add(new NCD8Light(ADDRESS, 2, "RampPorch", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 3, "RampAwning", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 4, "RearPorch", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 5, "RearAwning", "Outside", 2));
    Device::add(new NCD8Light(ADDRESS, 6, "Piano", "Office", 2));
    //Device::add(new NCD8Light(ADDRESS, 7, "Unused", "Office", 2));

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
void loop() {
    IoT::loop();
}
