/******************************************************************
 Activities collection

 This object manages a collection of Activity objects.

 Multiple activities can be active at the same time, so it is
 important to combine their effects, in addition to stopping an
 activity in a manner that doesn't break activities that continue
 to be in effect.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-12-04: Refactor to behaviors
 2016-06-24: Initial version
 ******************************************************************/

#include "activities.h"
#include "LisleRV.h"

String globalVariable;

Activities::Activities()
{
    // Without this method, strange error is reported and build fails
    _numActivities     = 0;
    _isVariableExposed = false;
}

Activity *Activities::addActivity(String name, int value)
{
  // Update existing activity if it exists
  Activity *activity = getActivityWithName(name);
  if (activity != NULL) {
    activity->value = value;

  // If not, create a new activity
  } else {
    if (_numActivities < MAX_NUM_ACTIVITIES-1)
    {
      activity = new Activity(name,value);
      _activities[_numActivities++] = activity;
    }
  }
  buildActivityVariable();
  return activity;
}

Activity* Activities::getActivityByNum(int activityNum)
{
  Activity *activity = NULL;
  if (activityNum < _numActivities)
  {
    activity = _activities[activityNum];
  }
  return activity;
}

Activity* Activities::getActivityWithName(String name)
{
  for(int i=0; i<_numActivities; i++)
  {
      if(_activities[i]->name.equalsIgnoreCase(name)) {
        return _activities[i];
      }
  }
  return NULL;
}

int Activities::count()
{
  return _numActivities;
}

bool Activities::expose()
{
  _isVariableExposed = true;
  if(!Particle.variable(kActivitiesVariableName, globalVariable))
  {
    Serial.print("Unable to expose ");
    Serial.print(kActivitiesVariableName);
    Serial.println(" variable");
    return false;
  }
  return true;
}

void Activities::buildActivityVariable()
{
  String newVariable = "";
  for(int i=0; i<_numActivities; i++)
  {
    newVariable += _activities[i]->name;
    newVariable += ":";
    newVariable += String(_activities[i]->value);
    if (i < _numActivities-1) {
      newVariable += ",";
    }
  }
  if(newVariable.length() < kMaxVariableStringLength) {
    globalVariable = newVariable;
  } else {
    Serial.println("Variable is too long. Need to extend to a 2nd variable");
  }

}
