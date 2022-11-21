/**
 This is the primary class for the Patriot IoT library.
 
 It aggregates all the other classes, and provides
 a common API for adding and configuring devices.
 
 This class coordinates realtime events.
 If cloud enabled, it subscribes to Particle.io notifications, and
 distributes them to devices.
 
 To build this code:
 1. Update version number in library.properties
 2. "particle library upload"
 3. Add library to controller (if needed) or update version
 4. "particle library add iot"
 
 http://www.github.com/rlisle/Patriot
 
 Written by Ron Lisle
 
 BSD license, check LICENSE for more information.
 All text above must be included in any redistribution.
 
 */
#include "IoT.h"

// Static Variables
Device*      Device::_devices = NULL;
MQTTManager* IoT::_mqttManager = NULL;
bool IoT::_cloudEnabled = false;

/**
 * Begin gets everything going.
 * It must be called exactly once by the sketch
 *  Network may not be connected yet.
 */
void IoT::begin(String brokerIP, String controllerName, bool enableCloud)
{
    _cloudEnabled = enableCloud;

    Time.zone(-6);              // CST
    handleDaylightSavings();
    
    WiFi.on();
    WiFi.connect();

    if(_cloudEnabled == true) {
//        Particle.connect();   // Not needed if SYSTEM_MODE(AUTOMATIC) (default)
        
        // Expose particle.io variables
        Device::expose();
        
        // Subscribe doesn't require a connection before calling.
        Particle.subscribe(kPublishName, IoT::subscribeHandler, MY_DEVICES);
    }

    String connectID = controllerName + "Id";
    _mqttManager = new MQTTManager(brokerIP, connectID, controllerName);

}


void IoT::mqttPublish(String topic, String message)
{
    _mqttManager->publish(topic, message);
}

/**
 * Loop method must be called periodically,
 * typically from the sketch loop() method.
 */
void IoT::loop()
{
    Device::loopAll();
    
    _mqttManager->loop();
    
    dailyReset();
}

/**
 * Perform a reboot daily at 2:00 am
 * and it has been running at least 6 hours
 */
//TODO: Is this necessary if we reset when MQTT is lost?
void IoT::dailyReset() {
    if(Time.hour() == 2 && System.uptime() > 60*60*6 ) {
        System.reset(RESET_NO_WAIT);
    }
}

/**
 * Particle.io Subscribe Handler
 * t:patriot m:<device>:<value>
 * This method handles commands from Alexa
 */
void IoT::subscribeHandler(const char *eventName, const char *rawData)
{
    String data = String(rawData).trim();
    String event(eventName);
    
    Log.info("Particle.io subscribe received data: '"+event+"', '"+data+"'");
    
    _mqttManager->parseMessage(event.toLowerCase(), data.toLowerCase());
}

/**
 MQTT Subscribe Handler
 */
void IoT::mqttHandler(char* rawTopic, byte* payload, unsigned int length) {
    
    _mqttManager->mqttHandler(rawTopic, payload, length);
}

/**
 publishValue()
 param: name of state
 param: value to assign state
 return: 0 success, -1 MQTT error
 */
void IoT::publishValue(String name, int value) {
    _mqttManager->publish("patriot/" + name, String(value));
}

// LATITUDE/LONGITUDE
//
void IoT::setLatLong(float latitude, float longitude) {
    // Currently only PartOfDay cares about (and persists) this.
    Device::setAllLatLong(latitude, longitude);
}

// TIMEZONE
//
void IoT::setTimezone(int timezone) {
    Log.trace("setTimezone: "+String(timezone));
    int8_t tz = timezone;
    Time.zone(float(timezone));
    // Persist this value across reboots
    EEPROM.put(TIMEZONE_ADDR, tz);
}

// from 2nd Sunday of March through 1st Sunday of November
// 2022 3/13 - 11/6, 2023 3/12 - 11/5, 2024 3/10 - 11/3
void IoT::handleDaylightSavings() {
    // Read & set persisted value from EEPROM (if present)
    int8_t timezone;
    EEPROM.get(TIMEZONE_ADDR, timezone);
    if(timezone == 0xff) {      // 0xff means never written
        timezone = -6;          // Default to CST
    }
    Log.trace("Setting timezone to "+String(timezone));
    Time.zone(float(timezone));
    
    int month = Time.month();
    if(month > 3 && month < 11) {
        Time.beginDST();
    } else if(month == 3) {
        handleDSTMarch();
    } else if(month == 11) {
        handleDSTNovember();
    }
}

void IoT::handleDSTMarch() {
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

void IoT::handleDSTNovember() {
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

