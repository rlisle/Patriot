/**
 MQTTManager.h
 
 This class handles all MQTT interactions.
 
 Note: to avoid making this a singleton,
 the caller must provide global callback handlers (see externs).
 
 http://www.github.com/rlisle/Patriot
 
 Written by Ron Lisle
 
 BSD license, check LICENSE for more information.
 All text above must be included in any redistribution.
 
 */
#pragma once
#include "Particle.h"
#include "MQTT.h"

class MQTTManager : public LogHandler
{
public:
    LogLevel    _logLevel;

    MQTTManager(String brokerIP, String connectID, String controllerName);
    
    bool        publish(String topic, String message);
    void        parseMessage(String topic, String message);
    void        loop();
    void        mqttHandler(char* topic, byte* payload, unsigned int length);
        
private:
    MQTT          *_mqtt;
    String        _controllerName;
    system_tick_t _lastMQTTtime;
    String        _connectID;       // Used when reconnecting
    
    int       _logging; // a counting semaphore to prevent recursion

    void      (*_callback)(char*,uint8_t*,unsigned int);
    void      connect(String connectID);
    void      reconnectCheck();
    int       parseValue(String message);
    void      parseLogLevel(String message);
    
    // LogHandler methods
    const char* extractFileName(const char *s);
    const char* extractFuncName(const char *s, size_t *size);
    void log(const char *category, String message);

protected:
    virtual void logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) override;
};
