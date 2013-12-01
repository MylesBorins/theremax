/*----------------------------------------------------------------------------
  MCD-X: General API for audio/graphics/interaction programming
         (sibling of MCD-Y API)

  Copyright (c) 2013 Ge Wang
    All rights reserved.
    http://ccrma.stanford.edu/~ge/

  Music, Computing, Design Group @ CCRMA, Stanford University
    http://ccrma.stanford.edu/groups/mcd/

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// name: x-fun.cpp
// desc: useful functions
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//          Jieun Oh (jieun5@ccrma.stanford.edu)
//    date: Winter 2010
//-----------------------------------------------------------------------------
#include "x-fun.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>

using namespace std;




//-----------------------------------------------------------------------------
// name: rand2i()
// desc: generates random int in [low, high)
//-----------------------------------------------------------------------------
long XFun::rand2i( long low, long high )
{
    // swap
    if( low > high )
        low ^= high; high ^= low; low ^= high;
    // diff
    long diff = high - low;
    // go
    return low + random() % diff;
}




//-----------------------------------------------------------------------------
// name: rand2f()
// desc: generates random double in [low, high]
//-----------------------------------------------------------------------------
double XFun::rand2f( double low, double high )
{
    // swap
    if( low > high ) {
        double temp = low; low = high; high = temp;
    }
    
    // diff
    double diff = high - low;
    // go
    return low + diff * random() / (double)0x7fffffff;
}




//-----------------------------------------------------------------------------
// name: srand()
// desc: seeds random
//-----------------------------------------------------------------------------
void XFun::srand()
{
    // seed
    srandom( (unsigned int)time( NULL ) );
}




//-----------------------------------------------------------------------------
// name: freq2midi()
// desc: converts frequency to midi notenum
//-----------------------------------------------------------------------------
double XFun::freq2midi( double freq )
{
    return (12.0 * log2(freq/440.0) + 57);
}




//-----------------------------------------------------------------------------
// name: midi2freq()
// desc: converts midi notenum to frequency
//-----------------------------------------------------------------------------
double XFun::midi2freq( double midi )
{
    return 440.0  * pow( 2.0, (midi - 57 ) / 12 );
}




//-----------------------------------------------------------------------------
// name: map()
// desc: map value in first range to value in second range
//-----------------------------------------------------------------------------
double XFun::map( double value, double min1, double max1, double min2, double max2 )
{
    return ((value - min1) / (max1-min1)) * (max2 - min2) + min2;
}




//-----------------------------------------------------------------------------
// name: clamp()
// desc: clamp value
//-----------------------------------------------------------------------------
double XFun::clamp( double value, double min, double max )
{
    // sanity check
    if( max < min )
    { double v = min; min = max; max = v; }
    
    if( value < min ) return min;
    if( value > max ) return max;
    
    return value;
}




//-----------------------------------------------------------------------------
// name: clampf()
// desc: clamp value
//-----------------------------------------------------------------------------
float XFun::clampf( float value, float min, float max )
{
    // sanity check
    if( max < min )
    { float v = min; min = max; max = v; }
    
    if( value < min ) return min;
    if( value > max ) return max;
    
    return value;
}




//-----------------------------------------------------------------------------
// name: tokenize()
// desc: string -> vector of substrings separated by delimiter
// adapted from:
// http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
//-----------------------------------------------------------------------------
void XFun::tokenize( const string & str, vector<string> & tokens,
                     const string & delimiters )
{
    // skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of( delimiters, 0 );
    // find first "non-delimiter".
    string::size_type pos = str.find_first_of( delimiters, lastPos );
    
    while( string::npos != pos || string::npos != lastPos )
    {
        // found a token, add it to the vector.
        tokens.push_back( str.substr( lastPos, pos - lastPos ) );
        // skip delimiters.  note the "not_of"
        lastPos = str.find_first_not_of( delimiters, pos );
        // find next "non-delimiter"
        pos = str.find_first_of( delimiters, lastPos );
    }
}




//-----------------------------------------------------------------------------
// name: toLower()
// desc: return a lower case version of a string
//-----------------------------------------------------------------------------
string XFun::toLower( const string & str )
{
    // make a local copy
    string foo = str;
    // do it
    std::transform( foo.begin(), foo.end(), foo.begin(), ::tolower );
    // return
    return foo;
}




//-----------------------------------------------------------------------------
// name: toUpper()
// desc: return an upper case version of a string
//-----------------------------------------------------------------------------
string XFun::toUpper( const string & str )
{
    // make a local copy
    string foo = str;
    // do it
    std::transform( foo.begin(), foo.end(), foo.begin(), ::toupper );
    // return
    return foo;
}




//-----------------------------------------------------------------------------
// name: diffTime()
// desc: calculates difference in seconds between current time and input time
//       - current time is automatically calculated
//       - input time is string in the format of "2010-05-19 11:16:42"
// usage: 	// test time
//       char* past = "2010-05-31 11:16:42"; 
//       long diff = XFun::diffTime( past);
//-----------------------------------------------------------------------------
long XFun::diffTime( const char * str )
{
    // cerr<< "diffTime received: " << str << endl;

    // current time
    char current[1024];
    struct tm tmc;
    memset( &tmc, 0, sizeof(struct tm) );
    // Wed May 26 14:57:59 2010
    char formatc[] = "%a %b %d %H:%M:%S %Y";
    time_t mytime = time(0);
    sprintf( current, "%s", asctime(localtime(&mytime)) );
    long diff = 0;

    // convert
    strptime( current, formatc, &tmc );

    // printf( "year: %d; month: %d; day: %d;\n", tmc.tm_year, tmc.tm_mon, tmc.tm_mday );
    // printf( "hour: %d; minute: %d; second: %d\n", tmc.tm_hour, tmc.tm_min, tmc.tm_sec );
    // printf( "week day: %d; year day: %d\n", tmc.tm_wday, tmc.tm_yday );

    // char str[] = "2010-05-19 11:16:42";
    char format[] = "%Y-%m-%d %H:%M:%S";
    struct tm tm;
    memset( &tm, 0, sizeof(struct tm) );

    // convert
    strptime( str, format, &tm );

    // printf( "year: %d; month: %d; day: %d;\n", tm.tm_year, tm.tm_mon, tm.tm_mday );
    // printf( "hour: %d; minute: %d; second: %d\n", tm.tm_hour, tm.tm_min, tm.tm_sec );
    // printf( "week day: %d; year day: %d\n", tm.tm_wday, tm.tm_yday );

    diff = difftime( mktime(&tmc), mktime(&tm) );

    // cerr<< "returning diffTime in seconds: " << diff << endl;
    return diff; 
}




//-----------------------------------------------------------------------------
// name: formatTime()
// desc: converts seconds to a string showing days/hours/minutes/seconds
// usage:        
//        cerr << "Difference in seconds: " << diff << endl;
//        cerr << "Formatted difference: " << XFun::formatTime(diff) << endl; 
//-----------------------------------------------------------------------------
std::string XFun::formatTime( long seconds, bool terse )
{    
    // cerr<< "received diffTime in seconds: " << seconds << endl;
    long secondsInDay = 3600*24;
    long days = seconds / secondsInDay;
    seconds %= secondsInDay;
    long secondsInHour = 3600;
    long hours = seconds / secondsInHour;
    seconds %= secondsInHour;
    long secondsInMinute = 60;
    long minutes = seconds / secondsInMinute;
    seconds %= secondsInMinute;

    char buffer[1024];
    // sprintf( buffer, "%i days, %i hours, %i minutes, %i seconds ago..\n", (int)days, (int)hours, (int)minutes, (int)seconds );
    
    // jieun: added
    if ( terse )
    {
        if (days > 0)
            sprintf( buffer, "%iday %ihr %imin %is ago..\n", (int)days, (int)hours, (int)minutes, (int)seconds );
        else if (hours > 0)
            sprintf( buffer, "%ihr %imin %is ago..\n", (int)hours, (int)minutes, (int)seconds );
        else if (minutes > 0)
            sprintf( buffer, "%imin %is ago..\n", (int)minutes, (int)seconds );
        else 
            sprintf( buffer, "%is ago..\n", (int)seconds );
        
        return buffer;
    }
    
    
    if (days > 0)
        sprintf( buffer, "%i days, %i hours, %i minutes, %i seconds ago..\n", (int)days, (int)hours, (int)minutes, (int)seconds );
    else if (hours > 0)
        sprintf( buffer, "%i hours, %i minutes, %i seconds ago..\n", (int)hours, (int)minutes, (int)seconds );
    else if (minutes > 0)
        sprintf( buffer, "%i minutes, %i seconds ago..\n", (int)minutes, (int)seconds );
    else 
        sprintf( buffer, "%i seconds ago..\n", (int)seconds );

    return buffer;
}
