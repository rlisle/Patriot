/**
 Patriot Awning

 This plugin supports connection to a Carefree BT12 automated awning.
 The awning opens when the <color> wire is connected to ground <color> wire for .5 second,
 and closes when <color> wire is connect to ground <color> wire for .5 seconds.
 Motion is stopped when the same wire is briefly connected again.

 This plugin is designed to coexist with up to 2 NCD4Relay
 and other NCD4Switch devices using the same board.
 
 Features:
 - 2 relay automated awning control
 - On (open), Off (closed), or timed partially open (1-99) using time.
 - Supports multiple boards
 
 http://www.github.com/rlisle/Patriot

 Written by Ron Lisle

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2023-05-28: Initial creation based on PatriotCurtain
*/

#pragma once

#include "Particle.h"
#include <device.h>

class Awning : public Device
{
 private:
    unsigned long _stopMillis;    // time to change start/stop pulse
    unsigned long _updateMillis;  // time to send next update msg
    unsigned long _startMillis;

    int8_t  _boardAddress;
    int8_t  _relayIndex;
    
    int8_t  _mode;  // 1 = close, 2 = open, 0 = idle
    int8_t  _stage; //1 = start pulse, 2=running, 3=stop pulse, 4=notDone
    bool    _holding;
    
    int8_t  _startPosition;

    void    pulse(bool start);
    bool    isAwningRunning();
    bool    isTimeToChangePulse();
    int     readCurrentState();
    int     currentPosition();
    
 public:
    Awning(int8_t boardAddress, int8_t relayIndex, String name, String room);
    
    void    begin();
    void    reset();
    void    setValue(int percent);  // Target state
    void    setHold(bool holding);
    void    loop();
};
