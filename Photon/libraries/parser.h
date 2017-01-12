/******************************************************************
 event parser

 Features:
 - Parses received events looking for match with those of interest.

 Events of interest are added to a linked list that is then used
 during comparisons.

 http://www.github.com/rlisle/rv-arduino-libraries

 Written by Ron Lisle, ron@lisles.net

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheets:

 Changelog:
 2016-07-21: Initial version
 ******************************************************************/

#ifndef parser_h
#define parser_h

class Parser
{
public:
  Parser();

  /* Return ID of added event */
  int       addEvent(const char *name);

private:
  int       _numEvents;
  String    _events[MAX_NUM_EVENTS];

}

#endif /* parser_h */
