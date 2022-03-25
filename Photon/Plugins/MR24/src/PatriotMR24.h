/******************************************************************
PatriotMR24 plugin

Features:

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

******************************************************************/
#pragma once

#include "Particle.h"
#include <device.h>

#define POLL_INTERVAL_MILLIS 500
#define TURNOFF_DELAY 2000

// Rx/Tx communications
#define MESSAGE_HEAD 0x55
#define ACTIVE_REPORT 0x04
#define FALL_REPORT 0x06

#define REPORT_RADAR 0x03
#define REPORT_OTHER 0x05

#define HEARTBEAT 0x01
#define ABNORMAL 0x02
#define ENVIRONMENT 0x05
#define BODYSIGN 0x06
#define CLOSE_AWAY 0x07

#define CA_BE 0x01
#define CA_CLOSE 0x02
#define CA_AWAY 0x03
#define SOMEBODY_BE 0x01
#define SOMEBODY_MOVE 0x01
#define SOMEBODY_STOP 0x00
#define NOBODY 0x00

// Return values
#define DETECTED_NOTHING -1
#define DETECTED_NOBODY 0
#define DETECTED_SOMEBODY_FAR 25
#define DETECTED_SOMEBODY 50
#define DETECTED_SOMEBODY_CLOSE 75
#define DETECTED_MOVEMENT 100


class MR24 : public Device
{
private:
    int     _s1pin;                // Use Rx/Tx if both 0
    int     _s2pin;                // "
    
    int     _s1value;
    int     _s2value;
    
    long    _lastPollTime;
    long    _lastMotion;
    
    int     _data[14];              // Typically 10 or 11 used
    int     _index;                 // Current position in _data
    int     _length;
    int     _function;
    int     _address1;
    int     _address2;
    int     _d1;
    int     _d2;
    int     _d3;
    unsigned _crc;

    int     _status;
    String  _statusMessage;
    String  _prevStatusMessage;
    
    bool    usingS1S2();
    bool    isTimeToCheckSensor();
    bool    didSensorChange();
    bool    didS1S2sensorChange();
    bool    didTxRxSensorChange();
    void    parseMessage();
    int     situation_judgment();
    void    sendScene(int scene);
    void    sendSensitivity(int sensitivity);
    int     calcCRC(unsigned char *data, int length);
    void    notify();
    void    logMessage();
    
public:
    MR24(int s1pin, int s2pin, String name, String room);
    
    void    begin();
    void    loop();
    
    String  status;    // String representation of last received data packet
    
    // Override to prevent MQTT from setting _percent.
    void    setValue(int percent) { return; };
};
