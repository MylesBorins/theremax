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
// name: x-audio.cpp
// desc: audio i/o abstraction
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "x-audio.h"
#include "RtAudio.h"
#include <iostream>
using namespace std;




// static instantiation
RtAudio * XAudioIO::o_audio;
XAudioCallback XAudioIO::o_callback;
SAMPLE * XAudioIO::o_input_buffer;
SAMPLE * XAudioIO::o_output_buffer;
unsigned int XAudioIO::o_num_frames;
unsigned int XAudioIO::o_num_channels;
unsigned int XAudioIO::o_srate;




//-----------------------------------------------------------------------------
// name: audio_callback()
// desc: audio callback function
//-----------------------------------------------------------------------------
static int audio_callback(
    void * outputBuffer, void * inputBuffer, unsigned int numFrames,
    double streamTime, RtAudioStreamStatus status, void * data )
{
    // check status
    if( status ) cerr << "[x-audio]: overflow/underflow detected..." << endl;

    // call to XAudioIO
    return XAudioIO::cb( (SAMPLE *)outputBuffer, (SAMPLE *)inputBuffer, numFrames,
                         streamTime, data );
}




//-----------------------------------------------------------------------------
// name: cb()
// desc: internal callback (should not be used by client)
//-----------------------------------------------------------------------------
int XAudioIO::cb( SAMPLE * outputBuffer, SAMPLE * inputBuffer,
                  unsigned int numFrames, double streamTime, void * data )
{
    // check if callback
    if( !o_callback )
    {
        cerr << "[x-audio]: ERROR -- missing audio callback..." << endl;
        return 0;
    }
    
    // sanity check
    if( numFrames > o_num_frames )
    {
        cerr << "[x-audio]: ERROR -- larger than expected frame size..." << endl;
        return 0;
    }
    
    // copy
    memcpy( o_input_buffer, inputBuffer, sizeof(SAMPLE)*numFrames*o_num_channels );
    // call back
    o_callback( o_input_buffer, numFrames, data );
    // copy
    memcpy( o_output_buffer, o_input_buffer, sizeof(SAMPLE)*numFrames*o_num_channels );
    memcpy( outputBuffer, o_output_buffer, sizeof(SAMPLE)*numFrames*o_num_channels );

    return 0;
}




//-----------------------------------------------------------------------------
// name: init()
// desc: initialize audio system
//-----------------------------------------------------------------------------
bool XAudioIO::init( unsigned int inputDevice,
                     unsigned int outputDevice,
                     unsigned int srate,
                     unsigned int & frameSize,
                     unsigned int numChannels,
                     XAudioCallback cb,
                     void * userData )
{
    // check if already init
    if( o_audio != NULL )
    {
        // error message
        cerr << "[x-audio]: already initialized..." << endl;
        return false;
    }
    
    // instantiate rt audio
    o_audio = new RtAudio();

    // make param structs
    RtAudio::StreamParameters iParams, oParams;

    // copy
    o_srate = srate;
    o_num_frames = frameSize;
    o_num_channels = numChannels;

    // first available device
    iParams.deviceId = inputDevice;
    iParams.nChannels = o_num_channels;
    
    // first available device
    oParams.deviceId = outputDevice;
    oParams.nChannels = o_num_channels;

    try {
        // try to open stream
        o_audio->openStream( &oParams, &iParams, RTAUDIO_FLOAT32,
                             srate, &o_num_frames, &audio_callback, userData );
    } catch ( RtError& e ) {
        try { // again
            // HACK: bump the oparams device id (on some systems, default in/out devices differ)
            oParams.deviceId++;
            // try to open stream
            o_audio->openStream( &oParams, &iParams, RTAUDIO_FLOAT32,
                                srate, &o_num_frames, &audio_callback, userData );
        } catch( RtError & e ) {
            // error message
            cerr << "[x-audio]: cannot initialize real-time audio I/O..." << endl;
            cerr << "[x-audio]: | - " << e.getMessage() << endl;
            // clean up
            SAFE_DELETE( o_audio );
            // done
            return false;
        }
    }
    
    // allocate buffer
    o_input_buffer = new SAMPLE[o_num_frames*numChannels];
    o_output_buffer = new SAMPLE[o_num_frames*numChannels];
    // check it
    if( !o_input_buffer || !o_output_buffer )
    {
        // error
        cerr << "[x-audio]: cannot allocate audio buffer(s)..." << endl;
        // clean up
        SAFE_DELETE_ARRAY( o_input_buffer );
        SAFE_DELETE_ARRAY( o_output_buffer );
        o_num_frames = 0;
        // clean up
        SAFE_DELETE( o_audio );
        // done
        return false;
    }

    // set the callback
    o_callback = cb;
    
    // copy actual frame size
    frameSize = o_num_frames;

    return true;
}




//-----------------------------------------------------------------------------
// name: start()
// desc: start the real-time audio
//-----------------------------------------------------------------------------
bool XAudioIO::start()
{
    // check flag
    if( !o_audio )
    {
        cerr << "[x-audio]: trying to start uninitialized audio..." << endl;
        return false;
    }
    
    try {
        // try to start the stream
        o_audio->startStream();
    } catch ( RtError& e ) {
        // error message
        cerr << "[x-audio]: cannot start real-time audio I/O..." << endl;
        cerr << "[x-audio]: | - " << e.getMessage() << endl;
        return false;
    }
    
    return true;
}




//-----------------------------------------------------------------------------
// name: stop()
// desc: stop the real-time audio
//-----------------------------------------------------------------------------
void XAudioIO::stop()
{
    // check flag
    if( !o_audio )
    {
        cerr << "[x-audio]: trying to stop uninitialized audio..." << endl;
        return;
    }

    try {
        // try to stop the stream
        o_audio->stopStream();
    }
    catch ( RtError& e ) {
        // error message
        cerr << "[x-audio]: cannot start real-time audio I/O..." << endl;
        cerr << "[x-audio]: | - " << e.getMessage() << endl;
    }
}
