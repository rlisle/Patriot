/******************************************************************
State

This class represents a state, such as "Nobody Home",
"Watching TV", or "Night Time".

States are either On or Off. 

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-10: Initial creation
******************************************************************/
#pragma once

#include "application.h"  // Defines String

class State
{
private:

public:
  String    _name;
  bool      _isOn;

  State(String name, bool isOn);
};
