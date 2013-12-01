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
// name: y-waveform.cpp
// desc: audio visualizations
//
// name: Ge Wang (ge@ccrma.stanford.edu)
// date: spring 2013
//-----------------------------------------------------------------------------
#include "y-waveform.h"
#include <iostream>
using namespace std;




//-----------------------------------------------------------------------------
// name: YWaveform()
// desc: constructor
//-----------------------------------------------------------------------------
YWaveform::YWaveform()
{
    // zero out
    m_buffer = NULL;
    m_vertices = NULL;
    m_numFrames = 0;
    // default width and height
    m_width = 2;
    m_height = 1;
    
    // set slew
    m_iAlpha.set( 1, 1, 1 );
}




//-----------------------------------------------------------------------------
// name: ~YWaveform()
// desc: destructor
//-----------------------------------------------------------------------------
YWaveform::~YWaveform()
{
    // clean up
    cleanup();
}




//-----------------------------------------------------------------------------
// name: init()
// desc: initialize
//-----------------------------------------------------------------------------
bool YWaveform::init( unsigned int numFrames )
{
    // clean up first
    cleanup();
    
    // try to allocate
    m_buffer = new SAMPLE[numFrames];
    // check it
    if( !m_buffer )
    {
        // log
        cerr << "[y-waveform]: cannot allocate audio buffer..." << endl;
        cleanup();
        return false;
    }
    
    // allocate vertex buffer
    m_vertices = new XPoint2D[numFrames];
    // check it
    if( !m_vertices )
    {
        // log
        cerr << "[y-waveform]: cannot allocate vertex buffer..." << endl;
        cleanup();
        return false;
    }
    
    // set the buffer size
    m_numFrames = numFrames;
    
    return true;
}




//-----------------------------------------------------------------------------
// name: cleanup()
// desc: clean up
//-----------------------------------------------------------------------------
void YWaveform::cleanup()
{
    // check
    if( m_buffer != NULL ) SAFE_DELETE_ARRAY( m_buffer );
    if( m_vertices != NULL ) SAFE_DELETE_ARRAY( m_vertices );
    
    // zero out
    m_numFrames = 0;
}




//-----------------------------------------------------------------------------
// name: set()
// desc: copy data to be visualized
//-----------------------------------------------------------------------------
void YWaveform::set( const SAMPLE * monoBuffer, unsigned int numFrames )
{
    // sanity check
    if( !m_buffer ) return;
    
    // figure out how much to copy
    unsigned int howmuch = numFrames;
    // compare against buffer size
    if( howmuch > m_numFrames ) howmuch = m_numFrames;
    
    // copy it
    memcpy( m_buffer, monoBuffer, numFrames*sizeof(SAMPLE) );
    
    // generate vertices
    generate();
}




//-----------------------------------------------------------------------------
// name: update()
// desc: update
//-----------------------------------------------------------------------------
void YWaveform::update( YTimeInterval dt )
{
    // interpolate
    m_iAlpha.interp( dt );
    
    // set
    alpha = m_iAlpha.value;
    
    // cerr << "alpha: " << alpha << endl;
}




//-----------------------------------------------------------------------------
// name: render()
// desc: render
//-----------------------------------------------------------------------------
void YWaveform::render()
{
    // disable light
    glDisable( GL_LIGHTING );
    // set blend function
    glBlendFunc( GL_ONE, GL_ONE );
    // enable blend
    glEnable( GL_BLEND );
	
    // enable
    glEnableClientState( GL_VERTEX_ARRAY );

    // set color
    glColor4f( col.x, col.y, col.z, alpha );
    // set pointer
    glVertexPointer( 2, GL_FLOAT, 0, m_vertices );
    // draw it
    glDrawArrays( GL_LINE_STRIP, 0, m_numFrames );
    
    // disable client state
    glDisableClientState( GL_VERTEX_ARRAY );
    // disable blend
    glDisable( GL_BLEND );
}




//-----------------------------------------------------------------------------
// name: generate()
// desc: generate vertices
//-----------------------------------------------------------------------------
void YWaveform::generate()
{
    // sanity check
    if( !m_buffer ) return;

    SAMPLE x = -m_width/2;
    SAMPLE inc = m_width / m_numFrames;

    // iterate over buffer
    for( int i = 0; i < m_numFrames; i++ )
    {
        // x coordinate
        m_vertices[i].x = x; x += inc;
        // y coordinate
        m_vertices[i].y = m_buffer[i] * m_height;
    }
}
