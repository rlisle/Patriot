/******************************************************************
State

This class represents a state or condition, such as "watching tv",
"night", or "backdooropen" where value = 0 off, 100 on.
Or it could be a measurement such as KitchenTemp where the
value is 20 to +127, or a percentage 0 - 100.

A state can receive a start, stop, on, off, or value message:
<state>:0     (eg. tv:0)
<state>:100   (was "on", eg. "Booth Lamp:On")
<state>:value (eg. KitchenTemp:85


http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-14: Rename activity to state
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-12-09: Convert to use value instead of isStarted
2016-07-07: Remove dimming mode
2016-01-17: Initial version
******************************************************************/
#pragma once

#include "application.h"  // Defines String

class State
{
private:

public:
  String    _name;
  int       _value; // Depends on what it is: temperature or 0 = off, 100 = 0n, etc.

  State(String name, int value);
};
