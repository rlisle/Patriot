/******************************************************************
state

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2020-11-14: Rename activity to state
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-07-24: Initial version
******************************************************************/
#include "state.h"

State::State(String name, int value)
{
  Serial.print("New state: ");
  Serial.println(name);
  _value = value;
  _name = name;
}
