/**
MQTTManager.cpp

This class handles all MQTT interactions.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/
#include "MQTTManager.h"
#include "constants.h"
#include "device.h"
#include "IoT.h"

#define MQTT_TIMEOUT_SECONDS 60*16
#define MQTT_ALIVE_SECONDS 60*5

MQTTManager::MQTTManager(String brokerIP, String connectID, String controllerName)
{
    _controllerName = controllerName.toLowerCase();
    _logging = 0;

    // We'll want to start with ALL whenever modifying code.
    // Use MQTT to switch to error when done testing or vs. a vs.
    _logLevel = LOG_LEVEL_ALL;     // See particle doc for options
        
    //TODO: by default, just the "app" category is used.
    //const LogCategoryFilters &filters) : LogHandler(level, filters)

    _mqtt =  new MQTT((char *)brokerIP.c_str(), 1883, IoT::mqttHandler);
    _connectID = connectID;

    connect();
}

bool MQTTManager::connect() {

    //TODO: could this even happen? I don't think so.
    if(_mqtt == NULL) {
        Log.error("ERROR! MQTTManager: connect called but object null");
    }
    
    if(!WiFi.ready()) {
        WiFi.connect();
        if(!WiFi.ready()) {
            Serial.println("MQTT connect() WiFi not ready");
            return false;
        }
    }
    
    _lastMQTTtime = Time.now();
    _lastAliveTime = _lastMQTTtime;


    if(_mqtt->isConnected()) {
        Log.info("MQTT is connected, so disconnecting first");
        LogManager::instance()->removeHandler(this);
        _mqtt->disconnect();
    }

    _mqtt->connect(_connectID);
    if (_mqtt->isConnected()) {
        if(_mqtt->subscribe(kPublishName+"/#") == false) {
            Log.error("Unable to subscribe to MQTT " + kPublishName + "/#");
        }
    } else {
        // This won't do anything because our handler isn't connected yet.
        Serial.println("MQTT is NOT connected! Check MQTT IP address");
    }
    // Looks good, now register our MQTT LogHandler
    LogManager::instance()->addHandler(this);

    Log.info("MQTT Connected");
    return true;
}

/**
 * Send MQTT data
 */
bool MQTTManager::publish(String topic, String message) {
    if(!_mqtt->isConnected() || !WiFi.ready()) {
        connect();
    }
    
    if(_mqtt->isConnected() && WiFi.ready()) {
        _mqtt->publish(topic,message);
        return true;
    } else {
        Serial.println("MQTT not connected: didn't publish "+topic+", "+message);
    }
    return false;
}

void MQTTManager::loop()
{
    _mqtt->loop();
    
    if(_mqtt->isConnected()) {
        sendAlivePeriodically();
    } else {
        Serial.println("MQTT not connected");
        connect();
    }

    //TODO: only poll this periodically
    reconnectCheck();
}

void MQTTManager::sendAlivePeriodically() {
    system_tick_t secondsSinceLastAlive = Time.now() - _lastAliveTime;
    if(secondsSinceLastAlive > MQTT_ALIVE_SECONDS) {
        _lastAliveTime = Time.now();
        String time = Time.format(Time.now(), "%a %H:%M");
        publish("patriot/alive/"+_controllerName, time);
    }
}

void MQTTManager::reconnectCheck() {
    // Still starting up? (eg. connection not done yet)
    if(_lastMQTTtime == 0) return;
    
    system_tick_t secondsSinceLastMessage = Time.now() - _lastMQTTtime;
    if(secondsSinceLastMessage > MQTT_TIMEOUT_SECONDS) {
        Log.warn("Connection lost, reconnecting. _lastMQTTtime = " + String(_lastMQTTtime) + ", Time.now() = " + String(Time.now()));
        //TODO: Fix connect(). Until then, just reset the Photon
        //connect();    // This will perform a reconnect
        Log.warn("MQTT lost. Resetting");
        Device::resetAll();
        System.reset(RESET_NO_WAIT);
    }
}

void MQTTManager::mqttHandler(char* rawTopic, byte* payload, unsigned int length) {

    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = 0;
    String message(p);
    String topic(rawTopic);

    _lastMQTTtime = Time.now();

    parseMessage(topic.toLowerCase(), message.toLowerCase());
}

//Mark - Parser

void MQTTManager::parseMessage(String lcTopic, String lcMessage)
{
    // This creates an infinite loop. Don't do it.
    //log("Parser received: " + lcTopic + ", " + lcMessage, LogDebug);
    
    // New Protocol: patriot/<name>  <value>
    if(lcTopic.startsWith(kPublishName+"/")) {
        String subtopic = lcTopic.substring(kPublishName.length()+1);
        
        // Look for reserved names
        if(subtopic.startsWith("alive")) {              // ALIVE
            // Remainder of topic is controller name
            // message is timestamp
            // Ignore it.

        } else if(subtopic.startsWith("brightness")) {           // BRIGHTNESS patriot/brightness/<device> value
            Log.info("brightness received: "+subtopic+", msg: "+lcMessage);
            int value = lcMessage.toInt();
            String deviceName = parseDeviceName(subtopic);
            Log.info("deviceName: "+deviceName);
            Device *device = Device::get(deviceName);
            if( device != NULL && value > 0) {
                Log.info(_controllerName + " setting brightness = " + lcMessage);
                device->setBrightness(value);
                //TODO: publish acknowledgement
                
            }

        } else if(subtopic == "latlong") {             // LATLONG
            // Windsor, ON: 42.3149, -83.0364 (park: 42.14413, -82.94876)
            // Spanish Fort, AL: 30.6685° N, 87.9109° W
            // Bonifay, FL: 30.7919° N, 85.6797° W
            // White Springs, FL: 30.3297° N, 82.7590° W
            // Tampa, FL: 27.9506° N, 82.4572° W
            // Austin lat/long: 30.2672° N, 97.7431° W (30.266666, -97.733330)
            //                  30.28267 N, 97.63624 W via iPhone maps in office.
            // eg. float longitude = -97.63624;
            // Split out latitude & longitude
            int commaIndex = lcMessage.indexOf(',');
            if(commaIndex < 0) return;
            
            String latString = lcMessage.substring(0, commaIndex-1);
            String lonString = lcMessage.substring(commaIndex+1);

            //TODO: handle '-' because toFloat doc says it doesn't
            float latitude = latString.toFloat();
            float longitude = lonString.toFloat();
            Log.info("lat/long = " + String(latitude) + "," + String(longitude));
            if(latitude != 0 && longitude != 0) {
                Log.info("Setting lat/long: " + String(latitude) + "," + String(longitude));
                IoT::setLatLong(latitude,longitude);
            }
            
        } else if(subtopic == "log" || subtopic.startsWith("log/")) {   // LOG
            // Ignore it.

        } else if(subtopic.startsWith("loglevel")) {    // LOGLEVEL
            if(subtopic == "loglevel/"+_controllerName) {
                Log.warn(_controllerName + " setting logLevel = " + lcMessage);
                parseLogLevel(lcMessage);
            }
            
        } else if(subtopic == "memory") {           // MEMORY
            if(lcMessage == _controllerName) {
                publish( "debug/"+_controllerName, String::format("Free memory = %d", System.freeMemory()));
            }
            
        } else if(subtopic == "ping") {             // PING
            // Respond if ping is addressed to us
            if(lcMessage == _controllerName) {
                Log.info("Ping addressed to us");
                publish(kPublishName + "/pong", _controllerName);
            }
            
        } else if(subtopic == "pong") {             // PONG
            // Ignore it.
            
        } else if(subtopic == "query") {            // QUERY
            if(lcMessage == _controllerName || lcMessage == "all") {
                Log.info("Received query addressed to us");
                Device::publishStates();
            }
                
        } else if(subtopic == "reset") {            // RESET
            // Respond if reset is addressed to us
            if(lcMessage == _controllerName) {
                Log.info("Reset addressed to us");
                Device::resetAll();
                System.reset(RESET_NO_WAIT);
            }
                
        } else if(subtopic == "state") {
            // Ignore - deprecated
                
        } else if(subtopic == "timezone") {            // TIMEZONE
            // San Francisco/PST -8
            // Austin/CST -6
            // Windsor/EST -5
            Log.info("Received timezone: " + lcMessage);
            int timezone = -6;          // Default to Austin CST
            //handle '-' because toInt doc says it doesn't
            if(lcMessage.charAt(0) == '-') {
                timezone = 0 - lcMessage.substring(1).toInt();
            } else {
                timezone = lcMessage.toInt();
            }
            if(timezone != 0) {
                Log.info("Setting timezone to: " + String(timezone));
                IoT::setTimezone(timezone);
            } else {
                Log.error("Invalid timezone");
            }
            
        // DEVICE
        } else {
            
            Device *device = Device::get(subtopic);
            if( device != NULL ) {
                int value = 100;
                if(lcMessage == "on" || lcMessage == "true") {
                    value = device->brightness();
                } else if(lcMessage == "off" || lcMessage == "false") {
                    value = 0;
                } else {
                    value = lcMessage.toInt();
                }
                
                // Handle save/restore value
                Log.info("Parser setting device " + subtopic + " to " + String(value));
                device->setValue(value);
                
                Device::buildDevicesVariable();
                
//            } else {
//                Log.info("Parsed unknown subtopic "+subtopic);
//            } else {
//                Log.info("Not our device");
            }
        }
    } else {
        // Not addressed or recognized by us
        Log.error("Parser: Not our message: "+String(lcTopic)+" "+String(lcMessage));
    }
}

String MQTTManager::parseDeviceName(String subtopic)
{
    int slashIndex = subtopic.indexOf('/');
    if(slashIndex < 0) return "unknown";
    
    String deviceName = subtopic.substring(slashIndex+1);

    return deviceName;
}

void MQTTManager::parseLogLevel(String lcMessage) {
    LogLevel level = LOG_LEVEL_WARN;
    if (lcMessage == "none") level = LOG_LEVEL_NONE;         // 70
    else if (lcMessage == "error") level = LOG_LEVEL_ERROR;  // 50
    else if (lcMessage == "warn" || lcMessage == "warning") level = LOG_LEVEL_WARN;    // 40
    else if (lcMessage == "info") level = LOG_LEVEL_INFO;    // 30
    else if (lcMessage == "trace") level = LOG_LEVEL_TRACE;  // 1
    else if (lcMessage == "all") level = LOG_LEVEL_ALL;      // 1
    else return;

    _logLevel = level;
}

// The following methods are taken from Particle FW, specifically spark::StreamLogHandler.
// See https://github.com/spark/firmware/blob/develop/wiring/src/spark_wiring_logging.cpp
const char* MQTTManager::extractFileName(const char *s) {
    const char *s1 = strrchr(s, '/');
    if (s1) {
        return s1 + 1;
    }
    return s;
}

const char* MQTTManager::extractFuncName(const char *s, size_t *size) {
    const char *s1 = s;
    for (; *s; ++s) {
        if (*s == ' ') {
            s1 = s + 1;                                                                                                                         // Skip return type
        } else if (*s == '(') {
            break;                                                                                                                         // Skip argument types
        }
    }
    *size = s - s1;
    return s1;
}

// This method is how we are called by the LogManager
// Note that this allow us to filter messages passed by the Log class
//      but the log class filters everything below INFO, so we only see info, warn, and error.
void MQTTManager::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
    String s;

//    LOG_LEVEL_ALL = 1
//    LOG_LEVEL_TRACE = 1
//    LOG_LEVEL_INFO = 30
//    LOG_LEVEL_WARN= 40
//    LOG_LEVEL_ERROR = 50
//    LOG_LEVEL_NONE = 70
    if (level < _logLevel) {
        return;
    }
    
    // Source file
    if (attr.has_file) {
        s = extractFileName(attr.file);                                                                                 // Strip directory path
        s.concat(s);                                                                                 // File name
        if (attr.has_line) {
            s.concat(":");
            s.concat(String(attr.line));                                                                                                                         // Line number
        }
        if (attr.has_function) {
            s.concat(", ");
        } else {
            s.concat(": ");
        }
    }

    // Function name
    if (attr.has_function) {
        size_t n = 0;
        s = extractFuncName(attr.function, &n);                                                                                 // Strip argument and return types
        s.concat(s);
        s.concat("(): ");
    }

    // Level
    s.concat(levelName(level));
    s.concat(": ");

    // Message
    if (msg) {
        s.concat(msg);
    }

    // Additional attributes
    if (attr.has_code || attr.has_details) {
        s.concat(" [");
        // Code
        if (attr.has_code) {
            s.concat(String::format("code = %p", (intptr_t)attr.code));
        }
        // Details
        if (attr.has_details) {
            if (attr.has_code) {
                s.concat(", ");
            }
            s.concat("details = ");
            s.concat(attr.details);
        }
        s.concat(']');
    }

    //TODO: If MQTT not connected, write to Serial instead
//    Serial.println(s);
    log(category, s);
}

// This is our formatter. We can format messages however we want.
void MQTTManager::log(const char *category, String message) {
    String time = Time.format(Time.now(), "%a %H:%M");

    if(!_logging) {
        _logging++;
        publish("patriot/log/"+_controllerName, time + " " + message);
        _logging--;
    }
}

