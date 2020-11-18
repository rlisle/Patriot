/******************************************************************
States

This class represents a collection of State objects.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-24: Rename Patriot
2017-03-05: Convert to v2 particle library
2016-07-24: Initial version
2016-09-09: Add behaviors
******************************************************************/
#pragma once

#include "state.h"

//TODO: convert to singly linked list
#define MAX_NUM_STATES 64

class States
{
public:

  States();

  State*     addState(String name, int value);  // TODO: convert to bool - true if added?
//  State*     getStateByNum(int stateNum);   // Used for iterating through states
  State*     getStateWithName(String name);
  int        count();

  bool       expose();    // Huh?

private:
  uint8_t    _numStates;
  State*     _states[MAX_NUM_STATES];   // TODO: convert to linked list.
  bool       _isVariableExposed;        // Is this needed?
  String     *_ptrToVariable;           // Is this needed?

  void buildStateVariable();            // Should this be here?
};
