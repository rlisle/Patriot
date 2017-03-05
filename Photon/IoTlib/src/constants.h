/******************************************************************
Constants file

This file contains common constants used by the IoT library.

http://www.github.com/rlisle/ParticleIoT

Written by Ron Lisle, ron@lisles.net

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

Changelog:
2017-03-05: Convert to v2 particle library
2016-03-05: Initial version
******************************************************************/
#pragma once

/**
 * Default alive interval 900000 = 15 minutes
 * but set to 60000 = 1 minute for testing
 */
static const long kDefaultAliveInterval     = 60000;

/**
 * Default particle.io publish/subscribe event name
 * This can be changed by IoT::publishName()
 */
static const String kDefaultPublishName     = "iotEven";

/**
 * Default particle.io controller name
 * Each IoT controller must be uniquelly named
 * This can be changed by IoT::controllerName()
 */
static const String kDefaultControllerName  = "iotController";

static const String kSupportedActivitiesVariableName = "Supported";
static const String kActivitiesVariableName          = "Activities";
static const String kControllersVariableName         = "Controllers";
static const int    kMaxVariableStringLength         = 266;

//#define kDevicesVariableName              "Devices"
//#define kTestVariableName                 "Testing"
//#define kTestFunctionName                 "test"
