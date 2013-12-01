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
// name: x-gfx.mm
// desc: general graphics routines
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//          Jieun Oh (jieun5@ccrma.stanford.edu)
//    date: Spring 2010
//-----------------------------------------------------------------------------
#include "x-gfx.h"
#include <math.h>
#include <iostream>
using namespace std;


//-----------------------------------------------------------------------------
// name: isPointInTriangle2D()
// desc: point in triangle test (2D)
//-----------------------------------------------------------------------------
bool XGfx::isPointInTriangle2D( const Vector3D & p, const Vector3D & a, 
                                  const Vector3D & b, const Vector3D & c )
{
    Vector3D v0 = c - a;
    Vector3D v1 = b - a;
    Vector3D v2 = p - a;
    
    // Compute dot products
    GLfloat dot00 = v0*v0;
    GLfloat dot01 = v0*v1;
    GLfloat dot02 = v0*v2;
    GLfloat dot11 = v1*v1;
    GLfloat dot12 = v1*v2;
    
    // Compute barycentric coordinates
    GLfloat invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    GLfloat u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    GLfloat v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    
    // Check if point is in triangle
    return (u > 0) && (v > 0) && (u + v < 1);
}




//-----------------------------------------------------------------------------
// name: getCurrentTime()
// desc: get current time for simulation
//-----------------------------------------------------------------------------
double XGfx::getCurrentTime( bool fresh )
{
    if( fresh )
    {
        ourPrevTime = ourCurrTime;
        gettimeofday( &ourCurrTime, NULL );
    }
    
    return ourCurrTime.tv_sec + (double)ourCurrTime.tv_usec / 1000000;
}




//-----------------------------------------------------------------------------
// name: resetCurrentTime()
// desc: reset current time for simulation
//-----------------------------------------------------------------------------
void XGfx::resetCurrentTime()
{
    ourPrevTime.tv_sec = 0;
    ourPrevTime.tv_usec = 0;
}




//-----------------------------------------------------------------------------
// name: delta()
// desc: get current time delta for simulation
//-----------------------------------------------------------------------------
GLfloat XGfx::delta()
{
    double prev = ourPrevTime.tv_sec + (double)ourPrevTime.tv_usec / 1000000;
    double curr = ourCurrTime.tv_sec + (double)ourCurrTime.tv_usec / 1000000;
    // first 0
    return (prev == 0 ? 0.0f : (GLfloat)(curr - prev)) * ourDeltaFactor;
}




//-----------------------------------------------------------------------------
// name: setDeltaFactor()
// desc: set time delta factor for simulation
//-----------------------------------------------------------------------------
void XGfx::setDeltaFactor( GLfloat factor )
{
    ourDeltaFactor = factor;
}




// static instantiation
struct timeval XGfx::ourCurrTime;
struct timeval XGfx::ourPrevTime;
GLfloat XGfx::ourDeltaFactor = 1.0f;
