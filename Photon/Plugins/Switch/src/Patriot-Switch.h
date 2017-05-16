#pragma once

/* Patriot-Switch library by Ron Lisle
 */

// This will load the definition for common Particle variable types
#include "Particle.h"

// This is your main class that users will import into their application
class PatriotSwitch
{
public:
  /**
   * Constructor
   */
  PatriotSwitch();

  /**
   * Example method
   */
  void begin();

  /**
   * Example method
   */
  void process();

private:
  /**
   * Example private method
   */
  void doit();
};
