/**
This is the primary class for the Patriot IoT library.

It aggregates all the other classes, and provides
a common API for adding and configuring devices.

This class coordinates realtime events.
It subscribes to Particle.io notifications, and distributes them to devices.

http://www.github.com/rlisle/Patriot

Device headers are all included at the bottom of this file.
 
Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
*/
#pragma once

#include "Particle.h"
#include "device.h"
#include "MQTTManager.h"

// PartOfDay
#define UNINITIALIZED 0
#define SUNRISE 1
#define MORNING 2
#define NOON 3
#define AFTERNOON 4
#define SUNSET 5
#define DUSK 6
#define NIGHT 7
#define DAWN 8

// Sleeping
#define AWAKE 1
#define RETIRING 2
#define ASLEEP 3

/**
 * Main IoT object.
 * It provides an API for creating and communicating with all others.
 * All methods and properties are static, meaning no initialization
 */
class IoT {

public:

    /**
     * begin(byte * brokerIP, String connectID)
     * Call begin to initialize the object.
     * This is done here instead of the constructor to improve debugging.
     **/
    static void begin(String brokerIP, String controllerName, bool mqttLogging);
    
    static String controllerName() { return _mqttManager->controllerName(); };
    
    static void startWatchdog();

    /**
     * Publish a message to MQTT
     * param: subtopic - MQTT topic prepended with "patriot/"
     * param message or value
     */
    static void publishMQTT(String subtopic, String message, bool retain = false);
    static void publishValue(String subtopic, int value, bool retain = false);

    /**
     * Loop needs to be called periodically
     */
    static void loop();
    
    
    static void setLatLong(float latitude, float longitude);
    static void setTimezone(int timezone);
    
    static void mqttHandler(char* topic, byte* payload, unsigned int length);
    static void subscribeHandler(const char *eventName, const char *rawData);

private:
    static String _controllerName;
    static MQTTManager* _mqttManager;
    static bool _mqttLogging;
    
    static void handleDaylightSavings();
    static void handleDSTMarch();
    static void handleDSTNovember();
};

// Device Declarations

class MCP23008 {
public:
    static int8_t _address;  //TODO: Add support for multiple boards
    static int8_t _iomap;
    static int8_t _current;
    static unsigned long _lastReadMillis;
    
    static int initialize(int address, int iomap);
    static int reset();
    static int read();
    static int write(int ioNum, bool value);
};
class PCA9634 {
public:
    static int8_t address;  //TODO: Add support for multiple boards
    
    static int  initialize(int boardAddress);
    static void reset();
    static void outputPWM(int lightNum, int level); // level 0-255
};
class PCA9685 {
public:
    static int8_t address;  //TODO: Add support for multiple boards
    
    static void initialize(int address);
    static void reset();
    static void outputPWM(int lightNum, int level); // level 0-4095
};

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

    void    pulse(bool high);
    bool    isCurtainRunning();
    bool    isTimeToChangePulse();
    
 public:
    Curtain(int8_t relayIndex, String name, String room);
    
    void    begin();
    void    setValue(int percent);  // Target state
    void    setHold(bool holding);
    void    loop();
};
class Light : public Device {
 private:
    int       _pin;
    int       _durationMSecs;
    int       _pinResolution;
    int       _maxLevel;
    int       _targetPercent;
    float     _currentPercent;
    float     _incrementPerMSec;
    long      _lastUpdateTime;

    int8_t    initializePin();
    void      outputPWM(float value);
    void      startSmoothDimming();
    int       convertToPinResolution(float percent);
    bool      isPwmSupported(int pin);

 public:
    Light(int pin, String name, String room, int durationMSecs);
    void      begin();
    void      setValue(int value);
    void      loop();
};
class NCD4Relay : public Device {
 private:
    int8_t        _relayNum;
    static int8_t _currentState;
    static int8_t _address;           // Addresses of board

    void   initializeBoard();

 public:
    NCD4Relay(int8_t address, int8_t relayNum, String name, String room);
    
    void    begin();

    void    setValue(int value);
    void    setOn();
    void    setOff();

    void    loop();
};
class NCD4Switch : public Device {
 private:
    long    _lastPollTime;

    int8_t  _boardAddress;
    int8_t  _switchBitmap;
    
    bool      isSwitchOn();
    bool      isTimeToCheckSwitch();
    bool      didSwitchChange();
    void      notify();

 public:
    NCD4Switch(int8_t switchIndex, String name, String room);
    void    loop();
};
class NCD8Light : public Device {
 private:
    int8_t   _lightNum;                // Up to 8 lights supported
    int      _dimmingMSecs;
    float    _currentLevel;            // Use for smooth dimming transitions.
    float    _targetLevel;
    float    _incrementPerMillisecond;
    
    long     _lastUpdateTime;

    int8_t  initializeBoard();
    void    outputPWM();
    void    startSmoothDimming();
    int     convertTo255(int value);

 public:
    NCD8Light(int8_t lightNum, String name, String room, int8_t duration = 0);
    void    begin();
    void    reset();
    void    setValue(int percent);
    void    loop();
};
class NCD8Switch : public Device {
private:
    long    _lastPollTime;
    int8_t  _switchBitmap;

    bool      isSwitchOn();
    bool      isTimeToCheckSwitch();
    bool      didSwitchChange();
    void      notify();

public:
    NCD8Switch(int switchIndex, String name, String room);
    void    loop();
    
    // Override to prevent MQTT from setting _percent.
    // Needed because of no auto behavior.
    // Our own generated MQTT message will reset _percent back to 0.
    void setPercent(int percent) { return; };
};
class NCD16Light : public Device {
 private:
    int8_t  _lightNum;                 // Up to 16 lights supported, 0 based

    // Note: Photon didn't support Floating Point but Photon 2 does.
    // So maybe instead of using int (signed 32 bit) fixed point, we can use FP.
    // TODO: convert to FP - don't need the following (I think)
    // (Outputs are 12 bit unsigned, so Lower 19 bits truncated and sign discarded.)
    // (Using signed ints to simplify underflow (< 0))
    //
    int     _dimmingMSecs;              // Duration in msecs
    float   _currentLevel;              // 0x00000000 to 0x7fffffff
    float   _targetLevel;               // transitioning to level
    float   _incrementPerMillisecond;   // Pre-calculated to minimize loop operations
    
    unsigned long _lastUpdateTime;      // Time in msecs

    int8_t  initializeBoard();
    void    outputPWM();
    void    startSmoothDimming();
    int     convertTo4k(int value);     // Convert 0-100 to 0-4095 exponential curve

 public:
    // lightNum is passed 1 based (1-16) but converted to 0-based in _lightNum
    NCD16Light(int8_t lightNum, String name, String room, int8_t duration = 0);
    void    begin();
    void    reset();
    void    setValue(int percent);  // 0-100
    void    loop();
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
class Power : public Device {
 private:
    float   _powerUsage[2];
    
    void    notify();

 public:
    Power(String name, String room);
    
    void    loop();
    void    mqtt(String topic, String message);
};
class Switch : public Device {
private:
    int        _pin;
    long       _lastPollTime;
    int        _filter;

    bool      isTimeToCheckSwitch();
    bool      didSwitchChange();
    void      notify();
    
public:
    Switch(int pinNum, String name, String room);

    void begin();
    void loop();
    
    // Override to prevent MQTT from setting _percent.
    void setValue(int percent) { return; };
};
class Voltage : public Device {
 private:
    system_tick_t   _lastPollTime;
    int             _pinNum;
    float           _stepValue;
    float           _fixedPoint;

    bool    isTimeToReadVoltage();
    bool    didVoltageChange();
    void    notify();

 public:
    Voltage(int pinNum, String name, String room, float fullScale, int fixedPoint);
    
    void    begin();
    void    loop();
};
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
