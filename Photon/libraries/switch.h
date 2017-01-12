/******************************************************************
 switch control

 Features:
 - Read and debounce switch input
 - Optional dimming control mode

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-29: Refactor to separate switch and switches classes
             Add mapping to an activity
 2016-06-21: Photon version
 2016-02-3: Initial version
 ******************************************************************/

#ifndef switch_h
#define switch_h

#include "application.h"

class Switch
{
public:
    Switch(int pinNum, String name);
    bool       isOn();
    void       loop();

private:
    int        _pin;
    String     _name;
    int        _isOn;
    long       _lastReadTime;

    bool      isTimeToCheckSwitch();
    bool      didSwitchChange();
    void      notify();
};

#endif /* switch_h */
