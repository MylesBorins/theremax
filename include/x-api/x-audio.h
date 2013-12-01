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
// name: x-audio.h
// desc: audio i/o abstraction
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __MCD_X_AUDIO_H__
#define __MCD_X_AUDIO_H__

#include "x-def.h"




// audio sample define
typedef float SAMPLE;
// typedef for audio callback function
typedef void (* XAudioCallback)( SAMPLE * buffer, unsigned int numFrames, void * userData );

// forward reference
class RtAudio;




//-----------------------------------------------------------------------------
// name: XAudioIO
// desc: static audio I/O
//-----------------------------------------------------------------------------
class XAudioIO
{
public:
    // initialize audio system
    static bool init( unsigned int inputDevice,
                      unsigned int outputDevice,
                      unsigned int srate,
                      unsigned int & frameSize,
                      unsigned int numChannels,
                      XAudioCallback cb,
                      void * userData );
    // start the real-time audio
    static bool start();
    // stop the real-time audio
    static void stop();
    
public:
    // get sample rate
    static unsigned int srate() { return o_srate; }
    // get number of channels
    static unsigned int numChannels() { return o_num_channels; }
    // get framesize
    static unsigned int framesize() { return o_num_frames; }
    
public:
    // internal callback (should not be used by client)
    static int cb( SAMPLE * outputBuffer, SAMPLE * inputBuffer,
                   unsigned int numFrames, double streamTime, void * data );
    
protected:
    static RtAudio * o_audio;
    static XAudioCallback o_callback;
    static SAMPLE * o_input_buffer;
    static SAMPLE * o_output_buffer;
    static unsigned int o_num_frames;
    static unsigned int o_num_channels;
    static unsigned int o_srate;
};




#endif
