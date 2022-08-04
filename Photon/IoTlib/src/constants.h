/**
Constants file

This file contains common constants used by the Patriot IoT library.

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.

*/
#pragma once

// EEPROM addresses (2047 bytes total)
#define VERSION_ADDR 0
#define LATITUDE_ADDR 4
#define LONGITUDE_ADDR 8

static const int    kMaxVariableStringLength   = 864; // was 622 < v3.0.0
static const String kDevicesVariableName       = "Devices";
static const String kChecklistVariableName     = "Checklist";
static const String kStatusVariableName        = "Status";
static const String kPublishName               = "patriot";
