/**
Constants file

This file contains common constants used by the Patriot IoT library.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/
#pragma once

/**
 * Default alive interval 900000 = 15 minutes
 * but set to 60000 = 1 minute for testing
 */
static const long kDefaultAliveInterval     = 60000;

/**
 * Default particle.io controller name
 * Each IoT controller must be uniquely named
 * This can be changed by IoT::controllerName()
 */
static const String kDefaultControllerName  = "iotcontroller";

static const String kSupportedStatesVariableName     = "Supported";
static const String kStatesVariableName              = "States";
static const String kControllersVariableName         = "Controllers";
static const int    kMaxVariableStringLength         = 622;
static const String kDevicesVariableName             = "Devices";
static const int    kMaxNumberStates                 = 32;
static const String kPublishName                     = "patriot";
