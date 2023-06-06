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

enum NetworkStatus { Disconnected, WifiStarting, MqttStarting, MqttConnected, CloudConnected };

class MQTTManager : public LogHandler
{
public:
    LogLevel    _logLevel;

    MQTTManager(String brokerIP, String controllerName, bool mqttLogging);
    
    bool        publish(String topic, String message, bool retain = false);
    void        parsePatriotMessage(String topic, String message);
    void        parseMQTTMessage(String topic, String message);
    void        loop();
    
    String      controllerName() { return _controllerName; };
        
private:
    MQTT      *_mqtt;
    String    _controllerName;
    long      _lastMQTTtime;
    long      _lastAliveTime;
    NetworkStatus _networkStatus;
    unsigned long _lastBlinkTimeMs;
    int           _blinkPhase;
    
    bool      _mqttLogging;
    int       _logging; // a counting semaphore to prevent recursion
    
    void      connectMQTT();
    void      (*_callback)(char*,uint8_t*,unsigned int);
    int       parseValue(String message);
    String    parseDeviceName(String subtopic);
    void      parseLogLevel(String message);
    void      updateStatusPeriodically();
    void      updateStatusLed();
    void      checkNetworkStatus();
    void      manageNetwork();
    void      doReboot();
    void      sendAck(String deviceName, String command, String message);


    
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
