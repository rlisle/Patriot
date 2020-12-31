/**
MQTTManager.cpp

This class handles all MQTT interactions.

Note: to avoid making this a singleton, 
the caller must provide global callback handlers (see externs).

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/
#include "MQTTManager.h"
#include "constants.h"
#include "IoT.h"

extern void globalMQTTHandler(char *topic, byte* payload, unsigned int length);

MQTTManager::MQTTManager(String brokerIP, String connectID, String controllerName)
{
    _controllerName = controllerName;
    _logging = 0;
//    _logLevel = LOG_LEVEL_ERROR;
    _logLevel = LOG_LEVEL_ALL;    // DEBUGGING ONLY!!!

    Time.zone(-6.0);
    
    //TODO: do we need this, and what should we pass?
    //const LogCategoryFilters &filters) : LogHandler(level, filters)

    _mqtt =  new MQTT((char *)brokerIP.c_str(), 1883, globalMQTTHandler);
    connect(connectID);
}

//TODO: If MQTT doesn't connect, then start 
void MQTTManager::connect(String connectID) {

    _lastMQTTtime = Time.now();

    if(_mqtt == NULL) {
        Log.error("ERROR! MQTTManager: connect called but object null");
    }

    if(_mqtt->isConnected()) {
        Log.info("MQTT is connected, so reconnecting...");
        _mqtt->disconnect();
    }

    _mqtt->connect(connectID);
    if (_mqtt->isConnected()) {
        if(_mqtt->subscribe(kPublishName+"/#") == false) {
            Log.error("Unable to subscribe to MQTT " + kPublishName + "/#");
        }
    } else {
        Log.error("MQTT is NOT connected! Check MQTT IP address");
    }
    // Looks good, not register our MQTT LogHandler
    LogManager::instance()->addHandler(this);

    Log.info("MQTT Connected");
    
}

bool MQTTManager::publish(String topic, String message) {
    if(_mqtt != NULL && _mqtt->isConnected()) {
        _mqtt->publish(topic,message);
        return true;
    }
    return false;
}

void MQTTManager::loop()
{
    if(_mqtt != NULL && _mqtt->isConnected()) {
        _mqtt->loop();
    }

    reconnectCheck();
}

void MQTTManager::reconnectCheck() {
//    system_tick_t secondsSinceLastMessage = Time.now() - _lastMQTTtime;
//    if(secondsSinceLastMessage > 5 * 60) {
//        log("WARNING: connection lost, reconnecting. _lastMQTTtime = " + String(_lastMQTTtime) + ", Time.now() = " + String(Time.now()));
//        connect();
//    }
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

// topic and messages are already lowercase
void MQTTManager::parseMessage(String topic, String message)
{
    // This creates an infinite loop. Don't do it.
    //log("Parser received: " + topic + ", " + message, LogDebug);
    
    // New Protocol: patriot/<name>  <value>
    if(topic.startsWith(kPublishName+"/")) {
        String subtopic = topic.substring(kPublishName.length()+1);
        
        // Look for reserved names
        // LOG
        if(subtopic.equals("log") || subtopic.startsWith("log/")) {
            // Ignore it.

        // LOGLEVEL
        } else if(subtopic.startsWith("loglevel")) {
            if(subtopic.equals("loglevel/"+_controllerName)) {
                Log.info(_controllerName + " setting logLevel = " + message);
                parseLogLevel(message);
            }
            
        // MEMORY
        } else if(subtopic.equals("memory")) {
            if(message.equals(_controllerName)) {
                Log.info( String::format("Free memory = %d", System.freeMemory()));
            }
            
        // PING
        } else if(subtopic.equals("ping")) {
            // Respond if ping is addressed to us
            if(message.equals(_controllerName)) {
                Log.trace("Ping addressed to us");
                _mqtt->publish(kPublishName + "/pong", _controllerName);
            }
            
        // PONG
        } else if(subtopic.equals("pong")) {
            // Ignore it.
            
        // RESET
        } else if(subtopic.equals("reset")) {
            // Respond if reset is addressed to us
            if(message.equals(_controllerName)) {
                Log.info("Reset addressed to us");
                Device::resetAll();
                System.reset(RESET_NO_WAIT);
            }
                
        // UNKNOWN
        } else {
            
            int percent = parseValue(message);
            Log.info("Parser setting state " + subtopic + " to " + message);
            IoT *iot = IoT::getInstance();
            // Is this a device name?
            Device *device = Device::getDeviceWithName(subtopic);
            if( device != NULL ) {
                device->setPercent(percent);
            } else {
                iot->setStateValue(subtopic,percent);
            }
        }
    } else {
        // Not addressed or recognized by us
        Log.error("Parser: Not our message: "+String(topic)+" "+String(message));
    }
}

int MQTTManager::parseValue(String message)
{
    if(message.equals("on")) {
        return 100;
    } else if(message.equals("off")) {
        return 0;
    }
    return message.toInt();
}

void MQTTManager::parseLogLevel(String message) {
    LogLevel level = LOG_LEVEL_ERROR;
    if (message.equals("none")) level = LOG_LEVEL_NONE;         // 70
    else if (message.equals("error")) level = LOG_LEVEL_ERROR;  // 50
    else if (message.equals("warn")) level = LOG_LEVEL_WARN;    // 40
    else if (message.equals("info")) level = LOG_LEVEL_INFO;    // 30
    else if (message.equals("trace")) level = LOG_LEVEL_TRACE;  // 1
    else if (message.equals("all")) level = LOG_LEVEL_ALL;      // 1
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
void MQTTManager::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
    String s;

//    LOG_LEVEL_ALL = 1
//    LOG_LEVEL_TRACE = 1
//    LOG_LEVEL_INFO = 30
//    LOG_LEVEL_WARN= 40
//    LOG_LEVEL_ERROR = 50
//    LOG_LEVEL_NONE = 70
    if (level < _logLevel) return;
    
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

//    if(!_logging && strcmp(category, "app") == 0) {
    if(!_logging) {
        _logging++;
        publish("patriot/log/"+_controllerName, time + " " + message);
        _logging--;
    }
}

