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
// name: y-fluidsynth.cpp
// desc: fluidsynth software synthesizer wrapper
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//    date: Winter 2010
//    version: 1.0
//-----------------------------------------------------------------------------
#include "y-fluidsynth.h"
#include <iostream>
using namespace std;




//-----------------------------------------------------------------------------
// name: YFluidSynth()
// desc: constructor
//-----------------------------------------------------------------------------
YFluidSynth::YFluidSynth()
    : m_settings(NULL), m_synth(NULL)
{ }




//-----------------------------------------------------------------------------
// name: ~YFluidSynth()
// desc: destructor
//-----------------------------------------------------------------------------
YFluidSynth::~YFluidSynth()
{
    // lock
    m_mutex.acquire();

    // clean up
    if( m_synth ) delete_fluid_synth( m_synth );
    if( m_settings ) delete_fluid_settings( m_settings );
    m_synth = NULL;
    m_settings = NULL;
    
    // unlock
    m_mutex.release();
}




//-----------------------------------------------------------------------------
// name: init()
// desc: init the synth
//-----------------------------------------------------------------------------
bool YFluidSynth::init( int srate, int polyphony )
{
    if( m_synth != NULL )
    {
        std::cerr << "synth already initialized..." << std::endl;
        return false;
    }
    
    // lock
    m_mutex.acquire();
    
    // log
    cerr << "[y-fluidsynth]: initializing synth..." << endl;
    // instantiate settings
    m_settings = new_fluid_settings();
    // set sample rate
    fluid_settings_setnum( m_settings, (char *)"synth.sample-rate", (double)srate );
    // set polyphony
    if( polyphony <= 0 ) polyphony = 1;
    else if( polyphony > 256 ) polyphony = 256;
    fluid_settings_setint( m_settings, (char *)"synth.polyphony", polyphony );
    // instantiate the synth
    m_synth = new_fluid_synth( m_settings );
    
    // unlock
    m_mutex.release();
    
    return m_synth != NULL;
}
    



//-----------------------------------------------------------------------------
// name: load()
// desc: load a font
//-----------------------------------------------------------------------------
bool YFluidSynth::load( const char * filename, const char * extension )
{
    if( m_synth == NULL ) return false;

    // lock
    m_mutex.acquire();

    // the pathc
    std::string path = filename;
    
    // log
    // NSLog( @"loading font file: %s.%s...", filename, extension );
    
    // load
    if( fluid_synth_sfload( m_synth, path.c_str(), true ) == -1 )
    {
        // error
        std::cerr << "cannot load font file: " << filename << "." << extension << std::endl;
        // unlock
        m_mutex.release();

        return false;
    }
    
    // unlock
    m_mutex.release();
    
    return true;
}




//-----------------------------------------------------------------------------
// name: programChange()
// desc: apply program change
//-----------------------------------------------------------------------------
void YFluidSynth::programChange( int channel, int program )
{
    if( m_synth == NULL ) return;
    if( program < 0 || program > 127 ) return;
    m_mutex.acquire();
    fluid_synth_program_change( m_synth, channel, program );
    m_mutex.release();
}
    



//-----------------------------------------------------------------------------
// name: controlChange()
// desc: control change
//-----------------------------------------------------------------------------
void YFluidSynth::controlChange( int channel, int data2, int data3 )
{
    if( m_synth == NULL ) return;
    if( data2 < 0 || data2 > 127 ) return;
    m_mutex.acquire();
    fluid_synth_cc( m_synth, channel, data2, data3 );
    m_mutex.release();
}




//-----------------------------------------------------------------------------
// name: noteOn()
// desc: send a note on message
//-----------------------------------------------------------------------------
void YFluidSynth::noteOn( int channel, float pitch, int velocity )
{
    // sanity check
    if( m_synth == NULL ) return;
    // integer pitch
    int pitch_i = (int)(pitch + .5f);
    // difference
    float diff = pitch - pitch_i;
    // lock
    m_mutex.acquire();
    // if needed
    if( diff != 0 )
    {
        // pitch bend
        fluid_synth_pitch_bend( m_synth, channel, (int)(8192 + diff * 8191) );
    }
    // sound note
    fluid_synth_noteon( m_synth, channel, pitch, velocity );
    // unlock
    m_mutex.release();
}




//-----------------------------------------------------------------------------
// name: pitchBend()
// desc: send a pitchBend on message
//-----------------------------------------------------------------------------
void YFluidSynth::pitchBend( int channel, float pitchDiff )
{
    // sanity check
    if( m_synth == NULL ) return;
    // lock
    m_mutex.acquire();
    // pitch bend
    fluid_synth_pitch_bend( m_synth, channel, (int)(8192 + pitchDiff * 8191) );
    // unlock
    m_mutex.release();
}




//-----------------------------------------------------------------------------
// name: noteOff()
// desc: send a note off message
//-----------------------------------------------------------------------------
void YFluidSynth::noteOff( int channel, int pitch )
{
    if( m_synth == NULL ) return;
    m_mutex.acquire();
    fluid_synth_noteoff( m_synth, channel, pitch );
    m_mutex.release();
}




//-----------------------------------------------------------------------------
// name: allNotesOff()
// desc: send all notes off message
//-----------------------------------------------------------------------------
void YFluidSynth::allNotesOff( int channel )
{
    // send all notes off control message
    controlChange( channel, 120, 0x7B );
}




//-----------------------------------------------------------------------------
// name: synthesize2()
// desc: synthesize stereo output (interleaved)
//-----------------------------------------------------------------------------
bool YFluidSynth::synthesize2( float * buffer, unsigned int numFrames )
{
    if( m_synth == NULL ) return false;
    m_mutex.acquire();
    // get it from fluidsynth
    int retval = fluid_synth_write_float( m_synth, numFrames, buffer, 0, 2, buffer, 1, 2 );
    m_mutex.release();
    
    // return
    return retval == 0;
}
