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
// name: gex-fluidsynth.h
// desc: fluidsynth software synthesizer wrapper
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//    date: Winter 2010
//    version: 1.0
//-----------------------------------------------------------------------------
#ifndef __MCD_Y_FLUIDSYNTH_H__
#define __MCD_Y_FLUIDSYNTH_H__

#include "fluidsynth.h"
#include "x-thread.h"




//-----------------------------------------------------------------------------
// name: class GeXFluidSynth
// desc: GeXFluidSynth class
//-----------------------------------------------------------------------------
class YFluidSynth
{
public:
    YFluidSynth();
    ~YFluidSynth();

public:
    // initialization
    bool init( int srate, int polophony );    
    // load a font
    bool load( const char * filename, const char * extension );

public:
    // program change
    void programChange( int channel, int program );
    // control change
    void controlChange( int channel, int data2, int data3 );
    // noteOn
    void noteOn( int channel, float pitch, int velocity );
    // pitchBend
    void pitchBend( int channel, float pitchDiff );
    // noteOff
    void noteOff( int channel, int pitch );
    // all notes off
    void allNotesOff( int channel );
    // synthesize (stereo)
    bool synthesize2( float * buffer, unsigned int numFrames );
    
protected:
    fluid_settings_t * m_settings;
    fluid_synth_t * m_synth;
    XMutex m_mutex;
};




#endif
