/**
 This is the primary class for the Patriot IoT library.
 
 It aggregates all the other classes, and provides
 a common API for adding and configuring devices.
 
 This class coordinates realtime events.
 If cloud enabled, it subscribes to Particle.io notifications, and
 distributes them to devices.
 
 To build this code:
 1. Update version number in library.properties
 2. "particle library upload" (uiot)
 3. Add library to controller (if needed) or update version
 4. "particle library add iot"
 
 http://www.github.com/rlisle/Patriot
 
 Written by Ron Lisle
 
 BSD license, check LICENSE for more information.
 All text above must be included in any redistribution.
 
 */
#include "IoT.h"
#include "constants.h"

// Global & Static Variables
Device*      Device::_devices = NULL;
MQTTManager* IoT::_mqttManager = NULL;

int          outOfMemory = -1;

void outOfMemoryHandler(system_event_t event, int param) {
    outOfMemory = param;
}

/**
 * Begin gets everything going.
 * It must be called exactly once by the sketch
 *  Network may not be connected yet.
 */
void IoT::begin(String brokerIP, String controllerName, bool mqttLogging)
{
    System.on(out_of_memory, outOfMemoryHandler);
//    Time.zone(-6);              // CT
    handleDaylightSavings();    // Set isDST appropriately
    
    // Expose particle.io variables
    Device::expose();
    
    // Start H/W Watchdog
    startWatchdog();
    
    // Start MQTT
    _mqttManager = new MQTTManager(brokerIP, controllerName, mqttLogging);
}

/**
 * Hardware Watchdog
 */
void IoT::startWatchdog()
{
    // Getting capabiltiies
    WatchdogInfo info;
    Watchdog.getInfo(info);

    // Get the capabilities that are always enabled
    WatchdogCaps mandatoryCaps = info.mandatoryCapabilities();

    // Get the capabilities that can be turned off
    WatchdogCaps optionalCaps = info.capabilities();
    
    Watchdog.init(WatchdogConfiguration().timeout(300s));
    Watchdog.start();
}

/**
 * Loop method must be called periodically,
 * typically from the sketch loop() method.
 */
void IoT::loop()
{
    Device::loopAll();
    _mqttManager->loop();

    //TODO: enable only for Photon 2
    if (outOfMemory >= 0) {
        // An out of memory condition occurred - reset device.
        Log.error("out of memory occurred size=%d", outOfMemory);
        delay(2000);
        System.reset();
    }
    
    //TODO: Use this only for Photon 1
//    if(Time.hour() == 3 && System.uptime() > 24*60*60) {
//        Log.info("Performing daily reboot");
//        delay(500);
//        System.reset();
//    }
    
    Watchdog.refresh();
    
}

/**
 * Particle.io Subscribe Handler
 * t:patriot m:<device>:<value>
 * This method handles commands from Particle Cloud (was Alexa)
 */
void IoT::subscribeHandler(const char *eventName, const char *rawData)
{
    String data = String(rawData).trim();
    String event(eventName);
    
    Log.info("Particle.io subscribe received data: '"+event+"', '"+data+"'");
    
    _mqttManager->parsePatriotMessage(event.toLowerCase(), data.toLowerCase());
}

/**
 MQTT Subscribe Handler
 */
void IoT::mqttHandler(char* rawTopic, byte* payload, unsigned int length)
{
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = 0;
    String message(p);
    String lcMessage = message.toLowerCase();
    String topic(rawTopic);
    String lcTopic = topic.toLowerCase();

    if(topic.startsWith("log")) {
        return;
    }
    _mqttManager->parseMQTTMessage(lcTopic, lcMessage);
    Device::mqttAll(lcTopic, lcMessage);
}

/**
 * Publish an MQTT message
 */
void IoT::publishMQTT(String subtopic, String message, bool retain)
{
    _mqttManager->publish(kPublishName+"/"+subtopic, message, retain);
}

/**
 Publish an integer value to MQTT patriot/<subtopic>
 param: subtopic patriot/<subtopic>
 param: mqtt message
 return: 0 success, -1 MQTT error
 */
void IoT::publishValue(String subtopic, int value, bool retain) {
    publishMQTT(subtopic, String(value), retain);
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
    EEPROM.get(TIMEZONE_ADDR, timezone);    // Photon2 uses a file on the Flash file system
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
    Log.trace("Begin DST");
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
    Log.trace("Begin DST");
    Time.beginDST();
}

