/******************************************************************
State

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-10: Initial version
******************************************************************/
#include "state.h"

State::State(String name, bool isOn)
{
  Serial.print("New state: ");
  Serial.println(name);
  _isOn = isOn;
  _name = name;
}
