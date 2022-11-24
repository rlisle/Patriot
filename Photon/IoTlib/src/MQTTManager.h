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

enum NetworkStatus { Unknown, Wifi, Mqtt, Cloud, Ok };

class MQTTManager : public LogHandler
{
public:
    LogLevel    _logLevel;

    MQTTManager(String brokerIP, String controllerName, bool mqttLogging);
    
    bool        publish(String topic, String message);
    void        parseMessage(String topic, String message);
    void        loop();
    void        mqttHandler(char* topic, byte* payload, unsigned int length);
    
    String      controllerName() { return _controllerName; };
        
private:
    MQTT      *_mqtt;
    String    _controllerName;
    long      _lastMQTTtime;
    long      _lastAliveTime;
    NetworkStatus _status;
    unsigned long _lastBlinkTimeMs;
    int           _blinkPhase;

    bool      _mqttLogging;
    int       _logging; // a counting semaphore to prevent recursion

    void      (*_callback)(char*,uint8_t*,unsigned int);
    bool      connect();
    void      reconnectCheck();
    int       parseValue(String message);
    String    parseDeviceName(String subtopic);
    void      parseLogLevel(String message);
    void      sendAlivePeriodically();
    void      updateStatusLed();
    void      manageNetwork();
    void      doReboot();

    
    //DEBUG
    long _lastAliveFrontPanel;
    long _lastAliveLeftSlide;
    long _lastAliveRearPanel;
    long _lastAliveRonTest;
    
    
    // LogHandler methods
    const char* extractFileName(const char *s);
    const char* extractFuncName(const char *s, size_t *size);
    void log(const char *category, String message);

protected:
    virtual void logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) override;
};
