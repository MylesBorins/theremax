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
// name: x-gfx.h
// desc: general graphics routines
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//          Jieun Oh (ge@ccrma.stanford.edu)
//    date: Spring 2010
//-----------------------------------------------------------------------------
#ifndef __MCD_X_GFX_H__
#define __MCD_X_GFX_H__

#include "x-def.h"
#include "x-vector3d.h"
#include <sys/time.h>
#include <string>



//-----------------------------------------------------------------------------
// name: class XGfx
// desc: CV graphics functions
//-----------------------------------------------------------------------------
class XGfx
{
public:
    // get current time
    static double getCurrentTime( bool fresh = false );
    // reset current time tracking
    static void resetCurrentTime();
    // get delta
    static GLfloat delta();
    // set delta factor
    static void setDeltaFactor( GLfloat factor );
    // set desired framerate
    

public:
    // point in triangle test (2D)
    static bool isPointInTriangle2D( const Vector3D & pt, const Vector3D & a, 
                                     const Vector3D & b, const Vector3D & c );
    // point line projection
    static bool pointLineProject( const Vector3D & pt, const Vector3D & a,
                                 const Vector3D & b, Vector3D & result );

public:
    static struct timeval ourPrevTime;
    static struct timeval ourCurrTime;
    static GLfloat ourDeltaFactor;
};




//-----------------------------------------------------------------------------
// name: struct XPoint2D
// desc: ...
//-----------------------------------------------------------------------------
struct XPoint2D
{
    GLfloat x;
    GLfloat y;
    
    // constructor
    XPoint2D( GLfloat _x = 0, GLfloat _y = 0 ) { x = _x; y = _y; }
    // copy
    XPoint2D( const XPoint2D & rhs ) { x = rhs.x; y = rhs.y; }
};




//-----------------------------------------------------------------------------
// name: struct XPoint3D
// desc: ...
//-----------------------------------------------------------------------------
struct XPoint3D
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    
    // constructor
    XPoint3D( GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0 )
    { x = _x; y = _y; z = _z; }
    
    // copy
    XPoint3D( const XPoint3D & rhs )
    { x = rhs.x; y = rhs.y; z = rhs.z; }
};




//-----------------------------------------------------------------------------
// name: struct XPoint4D
// desc: ...
//-----------------------------------------------------------------------------
struct XPoint4D
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
    
    // constructor
    XPoint4D( GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0, GLfloat _w = 0 )
    { x = _x; y = _y; z = _z; w = _w; }
    
    // copy
    XPoint4D( const XPoint4D & rhs )
    { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; }
};




#endif
