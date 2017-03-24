/******************************************************************
alive control

Features:
- Periodically publish alive event
- The self registration code may use this to become aware of
this controller.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-06-21: Initial version
******************************************************************/
#pragma once

#include "application.h"    // needed for String
#include "constants.h"

class IoT;

class Alive {

public:
    Alive();

    void setPublishName(String publishName);
    void setControllerName(String controllerName);

    void loop();

private:
    long   _lastLoopTime;
    String _publishName;        // Local copy from IoT
    String _controllerName;     // Local copy from IoT
};
