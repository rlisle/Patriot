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

class MR24 : public Device
{
private:
    int     _s1pin;                // Use Rx/Tx if both 0
    int     _s2pin;                // "
    
    int     _s1value;
    int     _s2value;
    
    long    _lastPollTime;
    
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
