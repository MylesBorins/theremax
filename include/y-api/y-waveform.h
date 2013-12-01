/*----------------------------------------------------------------------------
  MCD-Y: higher-level objects for audio/graphics/interaction programming
         (sibling of MCD-X API)

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
// name: y-waveform.h
// desc: audio visualizations
//
// name: Ge Wang (ge@ccrma.stanford.edu)
// date: spring 2013
//-----------------------------------------------------------------------------
#ifndef __MCD_Y_WAVEFORM_H__
#define __MCD_Y_WAVEFORM_H__

#include "x-audio.h"
#include "x-gfx.h"
#include "y-entity.h"




//-----------------------------------------------------------------------------
// name: class YWaveform
// desc: mono waveform visualization
//-----------------------------------------------------------------------------
class YWaveform : public YEntity
{
public:
    // constructor
    YWaveform();
    // destructor
    ~YWaveform();

public:
    // initialize
    bool init( unsigned int numFrames );
    // clean up
    void cleanup();
    // set width
    void setWidth( GLfloat width ) { m_width = width; generate(); }
    // set height
    void setHeight( GLfloat height ) { m_height = height; generate(); }
    // get width
    GLfloat getWidth() const { return m_width; }
    // get height
    GLfloat getHeight() const { return m_height; }
    // fade
    void fade( GLfloat targetAlpha, GLfloat slew = 1 )
    { if( slew <= 0 ) m_iAlpha.updateSet(targetAlpha);
      else m_iAlpha.update( targetAlpha, slew ); }

public:
    // copy data to be visualized
    void set( const SAMPLE * monoBuffer, unsigned int numFrames );

public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();

protected:
    // generate vertices
    void generate();
    
protected:
    // buffer (mono)
    SAMPLE * m_buffer;
    // buffer size
    unsigned int m_numFrames;
    // vertex array
    XPoint2D * m_vertices;

    // width of waveform
    GLfloat m_width;
    // height of waveform
    GLfloat m_height;
    
    // for fading
    Vector3D m_iAlpha;
};




#endif
