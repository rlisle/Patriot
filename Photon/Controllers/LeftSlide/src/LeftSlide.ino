/**
Left Slide Controller
Description: This sketch controls the 2 lights and sensors in the left slide.
Author: Ron Lisle
 
  To update Photon:
    1. Edit this code
    2. Update IoT and plugins if needed
    3. "particle flash left_slide2 --target 5.4.1"

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
#include "math.h"
//#include <PatriotMR24.h>

#define CONTROLLER_NAME "LeftSlide"
#define MQTT_BROKER "192.168.50.33"
#define CONNECT_TO_CLOUD true
#define LIVINGROOM_MOTION_TIMEOUT 3*60

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

#define PIR_POLL_INTERVAL_MILLIS 500

#define MQTT_LOGGING true
//SerialLogHandler logHandler1(57600, LOG_LEVEL_ALL);

//bool couchPresenceFiltered = 0;
//long lastCouchPresence = 0;

bool livingRoomMotion = false;
long lastLivingRoomMotion = 0;

//int couchPresence = 0;

// Mark: - Plugin Headers
class Light : public Device {
 private:
    int       _pin;

    int       _targetValue;             // Same as _value?
    float     _dimmingDuration;
    float     _currentValue;          // Use for smooth dimming transitions
    float     _incrementPerMillisecond;

    long      _lastUpdateTime;
    bool      _isInverted;              // On state = LOW instead of default HIGH
    bool      _forceDigital;            // On/Off only, even if PWM supported

    void      startSmoothDimming();
    void      outputPWM();
    int       scalePWM(int percent);
    bool      isPwmSupported();

 public:
    Light(int pin, String name, String room, bool isInverted=false, bool forceDigital=false);
    void      begin();
    
    void      setValue(int value);      // Set light immediately

    void      setDimmingDuration(float duration);
    float     getDimmingDuration();

    void      loop();
};
class PIR : public Device {
private:
    int        _pin;
    long       _lastPollTime;
    long       _lastMotion;
    long       _timeoutMSecs;

    bool      isTimeToCheckSensor();
    bool      didSensorChange();
    int       stateWithDelayedOff();
    void      notify();
    
public:
    PIR(int pinNum, String name, String room, int timeoutSecs);

    void begin();
    void loop();
    
    // Override to prevent MQTT from setting _percent.
    void setValue(int percent) { return; };
};


// Mark: - Setup
void setup() {
    WiFi.selectAntenna(ANT_INTERNAL);
//    WiFi.useDynamicIP();
    IoT::begin(MQTT_BROKER, CONTROLLER_NAME, MQTT_LOGGING);
    
    // Create Devices
    // Sensors
    Device::add(new PIR(A0, "LivingRoomMotion", "Living Room", LIVINGROOM_MOTION_TIMEOUT));
    //Device::add(new MR24(0, 0, "CouchPresence", "Living Room"));    // Was D3, D4

    // Lights
    Device::add(new Light(A2, "Couch", "Living Room", 2));
    Device::add(new Light(A5, "LeftVertical", "Living Room", 2));
}

// Mark - Loop
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

// Mark: - Light Plugin
/**
 * Constructor
 * @param pinNum is the pin number that is connected to the light.
 * @param name String name used to address the light.
 * @param isInverted True if On = output LOW
 * @param forceDigital True if output On/Off only (even if pin supports PWM)
 */
Light::Light(int pinNum, String name, String room, bool isInverted, bool forceDigital)
        : Device(name, room),
          _pin(pinNum),
          _isInverted(isInverted),
          _forceDigital(forceDigital) {
    _targetValue             = 0;
    _currentValue            = 0.0;
    _incrementPerMillisecond = 0.0;
    _lastUpdateTime          = 0;
    _type                    = 'L';
}
void Light::begin() {
    _dimmingDuration = isPwmSupported() ? 2.0 : 0;
    pinMode(_pin, OUTPUT);
    int res = analogWriteResolution(_pin,12);
//    int maxFreq = analogWriteMaxFrequency(_pin);
    outputPWM();
}

/**
 * Set value
 * @param value Int 0 to 100
 */
void Light::setValue(int value) {
    if(_targetValue == value) {
        outputPWM();
        return;
    }

    _targetValue = value;
    if(_dimmingDuration == 0.0 || isPwmSupported() == false) {
        _value = value;
        outputPWM();

    } else {
        startSmoothDimming();
    }
}

/**
 * Start smooth dimming
 * Use float _currentValue to smoothly transition
 * An alternative approach would be to calculate # msecs per step
 */
void Light::startSmoothDimming() {
    if((int)_value == _targetValue){
        return;
    }
    _currentValue = _value;
    _lastUpdateTime = millis();
    float delta = _targetValue - _value;
    _incrementPerMillisecond = delta / (_dimmingDuration * 1000);
}

/**
 * Set dimming duration
 * This will only affect any future transitions
 * @param duration float number of seconds
 */
void Light::setDimmingDuration(float duration) {
    _dimmingDuration = duration;
}

/**
 * Get dimming duration
 * @return float number of dimming duration seconds
 */
float Light::getDimmingDuration() {
    return _dimmingDuration;
}

/**
 * loop()
 * Called periodically to perform dimming, polling, etc.
 */
void Light::loop() {
    // Is fading transition underway?
    if(_value == _targetValue) {
        return;
    }

    long loopTime = millis();
    float millisSinceLastUpdate = (loopTime - _lastUpdateTime);
    _currentValue += _incrementPerMillisecond * millisSinceLastUpdate;
    _value = _currentValue;
    if(_incrementPerMillisecond > 0) {
        if(_currentValue > _targetValue) {
            _value = _targetValue;
        }
    } else {
        if(_currentValue < _targetValue) {
            _value = _targetValue;
        }
    }
    _lastUpdateTime = loopTime;
    outputPWM();
};

/**
 * Set the output PWM value (0-255) based on 0-100 value
 */
void Light::outputPWM() {
    if(isPwmSupported()) {
        int pwm = scalePWM(_value);
        analogWrite(_pin, pwm);
    } else {
        bool isOn = _value > 49;
        bool isHigh = (isOn && !_isInverted) || (!isOn && _isInverted);
        digitalWrite(_pin, isHigh ? HIGH : LOW);
    }
}

/**
 * Convert 0-100 to 0-0x00ffffff (12 bit) exponential scale
 * 0 = 0, 100 = 0xffffff
 */
int Light::scalePWM(int value) {
    //TODO: This is too extreme. Adjust algorithm
    if (value == 0) return 0;
    if (value >= 100) return 0xffffff;
    
//    float base = 1.05697667;        // 255
    float base = 1.08673221;          // 4095
    float pwm = pow(base,value);
    if (pwm > 4095) {
        return(4095);
    }
    return (int) pwm;
}

/**
 * Is PWM pin?
 * @param pin number
 * @return bool true if pin supports PWM
 */
bool Light::isPwmSupported() {
    switch(_pin) {
        case D1:
        case MOSI:   // D15
        case SCK:   // D17
        case A2:
        case A5:
            return _forceDigital ? FALSE : TRUE;
        default:
            break;
    };
    return FALSE;
}

// Mark - PIR
/**
 * Constructor
 * @param pinNum int pin number that is connected to the sensor output
 * @param name  String name of the event to send when sensor changes
 * @param room String
 * @parm timeoutSecs #secs to keep on after motion stops (was msecs)
 */
PIR::PIR(int pinNum, String name, String room, int timeoutSecs)
        : Device(name, room),
        _pin(pinNum),
        _timeoutMSecs(timeoutSecs * 1000)
{
    _type  = 'M';
    _value = 0;
}

void PIR::begin() {
    pinMode(_pin, INPUT);
    _lastPollTime = millis();
    _lastMotion = 0 - _timeoutMSecs;
}

/**
 * loop()
 * This is called repeatedly to allow device to do its work.
 */
void PIR::loop()
{
    if (isTimeToCheckSensor())
    {
        if (didSensorChange())
        {
            notify();
        }
    }
}


// Private Helper Methods
/**
 * isTimeToCheckSensor()
 * @return bool if enough time has elapsed to sample switch again
 */
bool PIR::isTimeToCheckSensor()
{
    long currentTime = millis();
    if (currentTime < _lastPollTime + PIR_POLL_INTERVAL_MILLIS)
    {
        return false;
    }
    _lastPollTime = currentTime;
    return true;
}


/**
 * didSensorChange()
 *
 * @return bool if switch has changed since last reading
 */
bool PIR::didSensorChange()
{
    int prevValue = _value;
    _value = stateWithDelayedOff();
    return _value != prevValue;
}

/**
 * stateWithDelayedOff()
 *
 * @return bool state with delayed turn off
 */
int PIR::stateWithDelayedOff()
{
    int pinState = digitalRead(_pin);
    int newValue = pinState ? 100 : 0;
    
    if(newValue == 100) {
        _lastMotion = millis();
    
    // Turn off only after delay
    } else {
        if(_lastMotion + _timeoutMSecs > millis()) {
            newValue = 100;
        }
    }
    return newValue;
}

/**
 * notify()
 * Publish switch state
 */
void PIR::notify()
{
    String message = String(_value);
    IoT::publishMQTT(_name,message);
}


