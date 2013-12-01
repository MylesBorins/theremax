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
// name: x-fun.h
// desc: useful functions
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//          Jieun Oh (jieun5@ccrma.stanford.edu)
//    date: Winter 2010
//-----------------------------------------------------------------------------
#ifndef __MCD_X_FUN_H__
#define __MCD_X_FUN_H__

#include "x-def.h"
#include <string>
#include <vector>
#include <time.h>
#include <stdio.h>




//-----------------------------------------------------------------------------
// name: class XFun
// desc: static-only class to access common functions
//-----------------------------------------------------------------------------
class XFun
{
public:
    // random integer in [low, high]
    static long rand2i( long low, long high );
    // random double in [low, high]
    static double rand2f( double low, double high );
    // seed random
    static void srand();

    // frequency to midi
    static double freq2midi( double freq );
    // midi to frequency
    static double midi2freq( double midi );
    
    // map
    static double map( double value, double min1, double max1, double min2, double max2 );
    
    // clamp
    static double clamp( double value, double min, double max );
    static float clampf( float value, float min, float max );
    
    // tokenizer
    static void tokenize( const std::string & str, std::vector<std::string> & tokens,
                          const std::string & delimiters = " " );
    // return a lower case version of a string
    static std::string toLower( const std::string & str );
    // return an upper case version of a string
    static std::string toUpper( const std::string & str );
	
	// find difference in seconds between current time and input time
	static long diffTime( const char * str ); 
	static std::string formatTime( long seconds, bool terse = false ); 
};




#endif
