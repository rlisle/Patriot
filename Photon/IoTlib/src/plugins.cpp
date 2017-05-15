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
#include "plugins.h"

Plugins::Plugins() {
    // Without this method, strange error is reported and build fails
    _numPlugins = 0;
    _plugins = new Plugin *[MAX_NUM_PLUGINS];
}

// Returns non-zero if # devices exceeded
int Plugins::addPlugin(Plugin *plugin)
{
  Serial.println("Adding plugin: "+plugin->name());
  if (_numPlugins < MAX_NUM_PLUGINS-1)
  {
    _plugins[_numPlugins++] = plugin;
  } else {
    return -1;
  }
  return 0;
}

void Plugins::loop()
{
  for(int x=0; x < _numPlugins; x++) {
    _plugins[x]->loop();
  }
}

Plugin* Plugins::getPluginByNum(int pluginNum)
{
  if (pluginNum < _numPlugins)
  {
    return _plugins[pluginNum];
  }
  return NULL;
}

Plugin* Plugins::getPluginWithName(String name)
{
  for(int i=0; i<_numPlugins; i++)
  {
      if(_plugins[i]->name().equalsIgnoreCase(name)) {
        return _plugins[i];
      }
  }
  return NULL;
}
