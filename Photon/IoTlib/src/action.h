/******************************************************************
action class

This class is the abstract parent of classes that support actions

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-07-24: Initial version
******************************************************************/
#pragma once

#include "application.h"  // Defines String

class Action
{
public:
  virtual void set(String state) = 0;
  virtual void setOn() = 0;
  virtual void setOff() = 0;
  virtual bool isOn() = 0;
  virtual bool isOff() = 0;

};
