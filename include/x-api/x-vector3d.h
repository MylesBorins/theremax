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
// name: x-vector3d.h
// desc: vector3d class
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2009
//-----------------------------------------------------------------------------
#ifndef __MCD_X_VECTOR_3D_H__
#define __MCD_X_VECTOR_3D_H__

#include <math.h>

// OpenGL
#if defined(__APPLE__)
  #include <GLUT/glut.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/glut.h>
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif




//-----------------------------------------------------------------------------
// name: class Vector3D
// desc: 3d vector
//-----------------------------------------------------------------------------
class Vector3D
{
public:
    Vector3D( ) : x(0), y(0), z(0) { }
    Vector3D( GLfloat _x, GLfloat _y, GLfloat _z ) { set( _x, _y, _z ); }
    Vector3D( const Vector3D & other ) { *this = other; }
    ~Vector3D() { }
    
public:
    void set( GLfloat _x, GLfloat _y, GLfloat _z ) { x = _x; y = _y; z = _z; }
    void setAll( GLfloat val ) { x = y = z = val; }
    
public:
    GLfloat & operator []( int index )
    { if( index == 0 ) return x; if( index == 1 ) return y;
        if( index == 2 ) return z; return nowhere; }
    const GLfloat & operator []( int index ) const
    { if( index == 0 ) return x; if( index == 1 ) return y;
        if( index == 2 ) return z; return zero; }
    const Vector3D & operator =( const Vector3D & rhs )
    { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }
    
    Vector3D operator +( const Vector3D & rhs ) const
    { Vector3D result = *this; result += rhs; return result; }
    Vector3D operator -( const Vector3D & rhs ) const
    { Vector3D result = *this; result -= rhs; return result; }
    Vector3D operator *( GLfloat scalar ) const
    { Vector3D result = *this; result *= scalar; return result; }
    
    inline void operator +=( const Vector3D & rhs )
    { x += rhs.x; y += rhs.y; z += rhs.z; }
    inline void operator -=( const Vector3D & rhs )
    { x -= rhs.x; y -= rhs.y; z -= rhs.z; }
    inline void operator *=( GLfloat scalar )
    { x *= scalar; y *= scalar; z *= scalar; }
    
    // dot product
    inline GLfloat operator *( const Vector3D & rhs ) const
    { GLfloat result = x*rhs.x + y*rhs.y + z*rhs.z; return result; }
    // cross product
    inline Vector3D operator ^( const Vector3D & rhs ) const
    { return Vector3D( y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x ); }
    // magnitude
    inline GLfloat magnitude() const
    { return ::sqrt( x*x + y*y + z*z ); }
    inline GLfloat magnitudeSqr() const
    { return x*x + y*y + z*z; }
    // normalize
    inline void normalize()
    { GLfloat mag = magnitude(); if( mag == 0 ) return; *this *= 1/mag; }
    // 2d angles
    inline GLfloat angleXY() const
    { return ::atan2( y, x ); }
    inline GLfloat angleYZ() const
    { return ::atan2( z, y ); }
    inline GLfloat angleXZ() const
    { return ::atan2( z, x ); }
    
    // 2d: set from polar
    void setXYFromPolar( float magnitude, float degrees )
    { x = magnitude * ::cos( degrees * M_PI / 180.0 );
        y = magnitude * ::sin( degrees * M_PI / 180.0 ); }
    
public: // using the 3-tuple for interpolation
    inline void interp()
    { value = (goal-value)*slew + value; }
    inline void interp( GLfloat delta )
    { value = (goal-value)*slew*delta + value; }
    inline void interp2( GLfloat delta ) // time-invariant
    { value = (goal - value) * (1.0f - ::powf(1.0f - slew, delta)) + value; }
    inline void update( GLfloat _goal )
    { goal = _goal; }
    inline void update( GLfloat _goal, GLfloat _slew )
    { goal = _goal; slew = _slew; }
    inline void updateSet( GLfloat _goalAndValue )
    { goal = value = _goalAndValue; }
    inline void updateSet( GLfloat _goalAndValue, GLfloat _slew )
    { goal = value = _goalAndValue; slew = _slew; }
    
public:
    // either use as .x, .y, .z OR .value, .goal, .slew
    union { GLfloat x; GLfloat value; };
    union { GLfloat y; GLfloat goal; };
    union { GLfloat z; GLfloat slew; };
    
public:
    static GLfloat nowhere;
    static GLfloat zero;
};




//-----------------------------------------------------------------------------
// name: class iSlew3D
// desc: contains a slew of vectors for slewing a vector, accessible as .actual
//-----------------------------------------------------------------------------
class iSlew3D
{
public:
    iSlew3D( float slew = 0.5 ) : m_slewX(0, 0, slew), m_slewY(0, 0, slew), m_slewZ(0, 0, slew) { }
    iSlew3D( const Vector3D & o, float slew = 1 ) : m_slewX(o.x, o.x, slew), m_slewY(o.y, o.y, slew), m_slewZ(o.z, o.z, slew), m_actual(o) { }
    iSlew3D( GLfloat x, GLfloat y, GLfloat z, float slew = 0.5 ) : m_slewX(x, x, slew), m_slewY(y, y, slew), m_slewZ(z, z, slew), m_actual(x, y, z) { }
    
public:
    void set( const Vector3D & o )
    { m_actual = o; m_slewX.value = o.x; m_slewY.value = o.y; m_slewZ.value = o.z; }
    void setSlew( GLfloat slew )
    { m_slewX.slew = m_slewY.slew = m_slewZ.slew = slew; }
    
public:
    void interp()
    { m_slewX.interp(); m_slewY.interp(); m_slewZ.interp();
        m_actual.x = m_slewX.value; m_actual.y = m_slewY.value; m_actual.z = m_slewZ.value; }
    void interp( float delta )
    { m_slewX.interp( delta ); m_slewY.interp( delta ); m_slewZ.interp( delta );
        m_actual.x = m_slewX.value; m_actual.y = m_slewY.value; m_actual.z = m_slewZ.value; }
    void interp2( float delta )
    { m_slewX.interp2( delta ); m_slewY.interp2( delta ); m_slewZ.interp2( delta );
        m_actual.x = m_slewX.value; m_actual.y = m_slewY.value; m_actual.z = m_slewZ.value; }
    void update( Vector3D goal )
    { m_slewX.update( goal.x ); m_slewY.update( goal.y ); m_slewZ.update( goal.z ); }
    void update( Vector3D goal, float slew )
    { m_slewX.update( goal.x, slew ); m_slewY.update( goal.y, slew ); m_slewZ.update( goal.z, slew ); }
    void updateSet( Vector3D goalAndValue )
    { m_slewX.updateSet( goalAndValue.x ); m_slewY.updateSet( goalAndValue.y ); m_slewZ.updateSet( goalAndValue.z ); }
    void updateSet( Vector3D goalAndValue, float slew )
    { m_slewX.updateSet( goalAndValue.x, slew ); m_slewY.updateSet( goalAndValue.y, slew ); m_slewZ.updateSet( goalAndValue.z, slew ); }
    
public:
    const Vector3D & actual() const { return m_actual; }
    Vector3D & actual() { return m_actual; }
    
    const Vector3D & slewX() const { return m_slewX; }
    const Vector3D & slewY() const { return m_slewY; }
    const Vector3D & slewZ() const { return m_slewZ; }
    
public:
    static float slewForDuration( float duration, float margin = 0.05 )
    { if( duration > 0 ) return 1 - ::pow( margin, 1.0 / duration );
        else return 1; }
    
private:
    Vector3D m_slewX;
    Vector3D m_slewY;
    Vector3D m_slewZ;
    Vector3D m_actual;
};




#endif
