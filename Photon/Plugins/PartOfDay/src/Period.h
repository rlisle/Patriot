/**
 Period.h
 
 PatriotPartOfDay plugin
 
http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check license.txt for more information.
All text above must be included in any redistribution.

*/
#pragma once

class Period {
private:
    int     _hour;
    int     _minute;

public:
    Period(int hour = 0, int minute = 0);
    
    // minute can be < 0 or > 59 and will be corrected
    void    set(int hour, int minute);
    
    String  info();
    
    bool    operator ==(const Period& hm);
    bool    operator >(const Period& hm);
    bool    operator <(const Period& hm);
};
