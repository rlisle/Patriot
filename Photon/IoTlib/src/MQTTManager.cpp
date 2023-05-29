/**
 MQTTManager.cpp
 
 This class handles all MQTT interactions.
 
 http://www.github.com/rlisle/Patriot
 
 Written by Ron Lisle
 
 BSD license, check LICENSE for more information.
 All text above must be included in any redistribution.
 
 */
#include "MQTTManager.h"
#include "device.h"
#include "IoT.h"
#include "constants.h"

#define MQTT_TIMEOUT_SECONDS 60*16
#define MQTT_ALIVE_SECONDS 60*5
#define BLINK_INTERVAL  250

MQTTManager::MQTTManager(String brokerIP, String controllerName, bool cloudEnabled, bool mqttLogging)
{
    _networkStatus = Disconnected;
    _controllerName = controllerName.toLowerCase();
    _cloudEnabled = cloudEnabled;
    _mqttLogging = mqttLogging; //TODO: deprecate this. It's only needed in this method.
    _lastAliveTime = 0;
    _logging = 0;
    _lastBlinkTimeMs = 0;
    _blinkPhase = 0;
    
    // We'll want to start with ALL whenever modifying code.
    // Use MQTT to switch to error when done testing or vs. a vs.
    _logLevel = LOG_LEVEL_ALL;     // See particle doc for options
    //TODO: by default, just the "app" category is used.
    //const LogCategoryFilters &filters) : LogHandler(level, filters)
    
    // Setup blue LED for network status
    pinMode(D7, OUTPUT);    // Blue LED
    digitalWrite(D7, LOW);
    
    _mqtt =  new MQTT((char *)brokerIP.c_str(), 1883, IoT::mqttHandler);
    
    Log.info("Connecting to MQTT");
    
    _lastMQTTtime = Time.now();
//    _mqtt->subscribe("#");
    
    if(_mqttLogging) {
        LogManager::instance()->addHandler(this);
        Log.info("MQTT log handler added");
    }
    
    //TODO: detect loss of other controllers
    _lastAliveFrontPanel = Time.now();
    _lastAliveLeftSlide = Time.now();
    _lastAliveRearPanel = Time.now();
    _lastAliveRonTest = Time.now();
    
}

void MQTTManager::loop()
{
    _mqtt->loop();
    updateStatusLed();
    manageNetwork();
    sendAlivePeriodically();
}

void MQTTManager::manageNetwork()
{
    //TODO: Turn on WiFi if needed
    switch (_networkStatus)
    {
        case Disconnected:
            WiFi.on();
            WiFi.connect();
            _networkStatus = WifiStarting;
            break;
            
        case WifiStarting:
            if(WiFi.ready()) {
                connectMQTT();
                _networkStatus = MqttStarting;
            }
            break;
            
        case MqttStarting:
            if(_mqtt->isConnected()) {
                _mqtt->subscribe("#");
                _networkStatus = MqttConnected;
                if(_cloudEnabled) {
                    Particle.connect();
                }
                Log.info("MQTT connected");
            }
            break;
            
        case MqttConnected:
            if(_cloudEnabled) {
                if(Particle.connected()) {
                    Particle.subscribe(kPublishName, IoT::subscribeHandler, MY_DEVICES);
                    _networkStatus = CloudConnected;
                    Log.info("Cloud connected");
                }
            } else {
                //TODO: Once connected, check for disconnect, etc.
            }
            break;
            
        case CloudConnected:
            //TODO: Once connected, check for disconnect, etc.
            
            break;
            
        default: // unknown state
            Log.info("Unknown network status");
            break;
    }

    // If no MQTT received within timeout period then reboot
    if(Time.now() > _lastMQTTtime + MQTT_TIMEOUT_SECONDS) {
        Log.error("MQTT Timeout.");
        doReboot();
    }
}

void MQTTManager::connectMQTT() {
    const char *user = NULL;
    const char *pw = NULL;
    String willTopic = kPublishName + "/" + _controllerName + "/status";
    MQTT::EMQTT_QOS willQoS = MQTT::QOS0;
    uint8_t willRetain = 0;
    String willMessage = "Offline";
    bool clean = false;
    _mqtt->connect(_controllerName + "Id", user, pw, willTopic, willQoS, willRetain, willMessage, clean);
    _lastMQTTtime = Time.now();
}


void MQTTManager::sendAlivePeriodically() {
    if(Time.now() > _lastAliveTime + MQTT_ALIVE_SECONDS) {
        _lastAliveTime = Time.now();
        String time = Time.format(Time.now(), "%a %H:%M");
        publish(kPublishName+"/alive/"+_controllerName, time, false);
    }
}

void MQTTManager::doReboot() {
    Log.warn("Rebooting...");
    Device::resetAll();
    System.reset(RESET_NO_WAIT);
}

/**
 * Send MQTT data
 */
bool MQTTManager::publish(String topic, String message, bool retain) {
    if(_mqtt->isConnected() && WiFi.ready()) {
        _mqtt->publish(topic, (const uint8_t*)message.c_str(), message.length(), retain, retain ? MQTT::QOS1 : MQTT::QOS0);
        return true;
    } else {
        Log.warn("publish while MQTT not connected: " + topic + ", " + message);
    }
    return false;
}

//Mark - Parser

void MQTTManager::parseMQTTMessage(String lcTopic, String lcMessage)
{
    _lastMQTTtime = Time.now();

    if(lcTopic.startsWith(kPublishName)) {
        parsePatriotMessage(lcTopic.substring(8), lcMessage);   // Skip over "patriot/"
    }
    //TODO: if we wanted, we could do something with log messages, etc.
}

void MQTTManager::parsePatriotMessage(String lcTopic, String lcMessage)
{
    String subtopics[5];
    int start = 0;
    int end = lcTopic.indexOf('/');
    int numTopics = 0;
    if(end <= 0) return;    // guard agains just "patriot"
    do {
        subtopics[numTopics] = lcTopic.substring(start, end);
        start = end+1;
        end = lcTopic.indexOf('/', start);
        numTopics++;
    } while(numTopics < 4 && end > 0);
    subtopics[numTopics++] = lcTopic.substring(start);  // Last one
    
    if(numTopics > 0) {
        
        // ACK
        if(subtopics[0] == "ack") {                         // patriot/ack/<device>/<command>
            // Ignore Acknowledgements
            
            // ALIVE
        } else if(subtopics[0] == "alive" && numTopics > 1) {                // patriot/alive/<controller>
            
            //TODO: Refactor to allow unknown controller names (array)
            if(subtopics[1] == "frontpanel") {
                _lastAliveFrontPanel = Time.now();
            } else if(subtopics[1] == "leftslide") {
                _lastAliveLeftSlide = Time.now();
            } else if(subtopics[1] == "rearpanel") {
                _lastAliveRearPanel = Time.now();
            }
            
            // BRIGHTNESS
        } else if(numTopics > 1 && subtopics[1] == "brightness") {           // patriot/<device>/brightness value
            int value = lcMessage.toInt();
            String deviceName = subtopics[0];
            Device *device = Device::get(deviceName);
            if( device != NULL && value > 0) {
                device->setBrightness(value);
                sendAck(deviceName, "brightness", lcMessage);
            }
            
        // HOLD
        } else if(numTopics > 1 && subtopics[1] == "hold") {             // patriot/<device>/hold n/a
            Device *device = Device::get(subtopics[0]);
            if( device != NULL) {
                // message is currently ignored and can be anything
                Log.info(_controllerName + ": hold " + subtopics[0]);
                device->setHold(lcMessage != "0"); // not sure what message will be
            }
            

            // LATLONG
        } else if(subtopics[0] == "latlong") {                                  // patriot/latlong lat,long
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
            Log.trace("lat/long = " + String(latitude) + "," + String(longitude));
            if(latitude != 0 && longitude != 0) {
                Log.trace("Setting lat/long: " + String(latitude) + "," + String(longitude));
                IoT::setLatLong(latitude,longitude);
            }
            
            // LOGLEVEL
        } else if(subtopics[0] == "loglevel") {
            if(numTopics == 1 || subtopics[1] == _controllerName || subtopics[1] == "all" ) {
                Log.warn(_controllerName + " setting logLevel = " + lcMessage);
                parseLogLevel(lcMessage);
            }
            
            // MEMORY
        } else if(subtopics[0] == "memory") {
            if(lcMessage == _controllerName || lcMessage == "all") {
                Log.info(_controllerName + ": free memory = %d", System.freeMemory());
            }
            
            // QUERY
        } else if(subtopics[0] == "query") {
            if(lcMessage == _controllerName || lcMessage == "all") {
                Log.info(_controllerName + ": received query addressed to us");
                Device::publishStates();
            }
            
            // RESET
        } else if(subtopics[0] == "reset") {
            if(lcMessage == _controllerName || lcMessage == "all") {
                Log.info(_controllerName + ": reset addressed to us");
                Device::resetAll();
                System.reset(RESET_NO_WAIT);
            }
            
            // SET
        } else if(numTopics > 1 && subtopics[1] == "set") {             // patriot/<device>/set value
            Device *device = Device::get(subtopics[0]);
            if( device != NULL) {
                int value = lcMessage.toInt();  // 0 if not numerical
                if(lcMessage == "on" || lcMessage == "true") value = device->brightness();
                else if(lcMessage == "off" || lcMessage == "false") value = 0;
                device->setValue(value);
                sendAck(subtopics[0], "set", lcMessage);
            }
            
            // TIMEZONE
        } else if(subtopics[0] == "timezone") {
            // San Francisco/PST -8
            // Austin/CST -6
            // Windsor/EST -5
            Log.info(_controllerName + ": received timezone = " + lcMessage);
            int timezone = -6;          // Default to Austin CST
            //handle '-' because toInt doc says it doesn't
            if(lcMessage.charAt(0) == '-') {
                timezone = 0 - lcMessage.substring(1).toInt();
            } else {
                timezone = lcMessage.toInt();
            }
            if(timezone != 0) {
                Log.info(_controllerName + ": setting timezone to: " + String(timezone));
                IoT::setTimezone(timezone);
            } else {
                Log.error("Invalid timezone");
            }
        }
    }
}

void MQTTManager::sendAck(String deviceName, String command, String message) {
    publish(kPublishName + "/ack/" + deviceName + "/" + command, message);
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
    // As recommended by ScruffR, don't pass time argument
    String time = Time.format("%a %H:%M");
    
    if(!_logging) {
        _logging++;
        // Separate topic now, allows logging patriot/ messages without causing a loop
        publish("log/"+_controllerName, time + " " + message, false);
        _logging--;
    }
}

//TODO: move to its own class
// Status LEDs
// RGB LED
// Breathing white: Wifi off
// Breathing blue: Wifi on but not connected to network
// Breathing green: Connected to network but not Cloud
// Breathing Cyan: Cloud connected
// Breathing rainbow: Device Signal'ed
// Blinking Magenta: Safe mode, ready to flash, not running code.
//   - Hold setup while booting, release when magenta.
// Blinking green: connecting to Wifi
// Blinking cyan: connecting to Cloud
// Blinking blue: Listening mode
// Blinking yellow: DFU mode
//
// Blue LED
// 5 blinks: Disconnected
// 4 blinks: WiFi Starting to connect
// 3 blinks: MQTT Starting to connect
// 2 blinks: MQTT connected
// 1 blink:  Cloud connected
//
void MQTTManager::updateStatusLed() {
    
    if(millis() >= _lastBlinkTimeMs + BLINK_INTERVAL) {
        
        _lastBlinkTimeMs = millis();
        _blinkPhase++;
        
        int nextLed = LOW;
        
        switch (_networkStatus)
        {
            case Disconnected:  // 5 short blinks
                if(_blinkPhase == 1 || _blinkPhase == 3 || _blinkPhase == 5 || _blinkPhase == 7 || _blinkPhase == 9) {
                    nextLed = HIGH;
                } else if(_blinkPhase > 12) {
                    _blinkPhase = 0;
                }
                break;
            case WifiStarting:  // 4 short blinks
                if(_blinkPhase == 1 || _blinkPhase == 3 || _blinkPhase == 5 || _blinkPhase == 7) {
                    nextLed = HIGH;
                } else if(_blinkPhase > 10) {
                    _blinkPhase = 0;
                }
                break;
            case MqttStarting:  // 3 short blinks off, on, off, on, off, off
                if(_blinkPhase == 1 || _blinkPhase == 3 || _blinkPhase == 5) {
                    nextLed = HIGH;
                } else if(_blinkPhase > 8) {
                    _blinkPhase = 0;
                }
                break;
            case MqttConnected: // 2 short blink off, on, off, off
                if(_blinkPhase == 1 || _blinkPhase == 3) {
                    nextLed = HIGH;
                } else if(_blinkPhase > 8) {
                    _blinkPhase = 0;
                }
                break;
            case CloudConnected:    // 1 short blink
                if(_blinkPhase == 1) {
                    nextLed = HIGH;
                } else if(_blinkPhase > 8) {
                    _blinkPhase = 0;
                }
                break;
        }
        digitalWrite(D7, nextLed);
    }
}
