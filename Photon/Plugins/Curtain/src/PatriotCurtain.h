/**
 Patriot Curtain

 This plugin supports connection to an automated curtain.
 The curtain opens when pin 2 is connected to pin 1 ground,
 and closes when pin 4 is connect to pin 1 ground.
 (of possibly pins 5 & 6 instead. Need to test)

 This plugin is designed to coexist with up to 2 NCD4Relay
 and other NCD4Switch devices using the same board.
 
 Features:
 - 2 relay automated curtain control
 - On (open), Off (closed), or timed partially open (1-99)
 - Supports multiple boards
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2021-01-16: Initial creation based on NCD8Relay
*/

#pragma once

#include "Particle.h"
#include <device.h>

class Curtain : public Device
{
 private:
    unsigned long _stopMillis;    // time to stop
    unsigned long _doneMillis;    // time to signal done

    int8_t  _boardAddress;
    int8_t  _relayIndex;
    
    int8_t  _mode;  // 1 = close, 2 = open, 0 = idle
    int8_t  _stage; //1 = start pulse, 2=running, 3=stop pulse, 4=notDone
    bool    _holding;

    void    pulse(bool start);
    bool    isCurtainRunning();
    bool    isTimeToChangePulse();
    int     readCurrentState();
    
 public:
    Curtain(int8_t boardAddress, int8_t relayIndex, String name, String room);
    
    void    begin();
    void    reset();
    void    setValue(int percent);  // Target state
    void    setHold(bool holding);
    void    loop();
};
