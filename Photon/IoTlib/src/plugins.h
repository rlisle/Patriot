/******************************************************************
Plugins

This class represents a collection of plugins.
These allow extending the kinds of devices that are supported.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

Changelog:
2017-05-14: Add plugins
******************************************************************/
#pragma once

#include "plugin.h"

#define MAX_NUM_PLUGINS 8

class Plugins
{
public:

  Plugins();

  void        loop();
  int         addPlugin(Plugin *plugin);
  Plugin*     getPluginWithName(String name);
  Plugin*     getPluginByNum(int pluginNum);

private:
  uint8_t     _numPlugins;
  Plugins**    _plugins;
};
