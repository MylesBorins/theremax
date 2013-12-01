//-----------------------------------------------------------------------------
// name: theremax-audio.cpp
// desc: audio stuff for bokeh
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "theremax-audio.h"
#include "theremax-globals.h"
#include "theremax-sim.h"
#include "x-thread.h"
#include "y-fft.h"
#include "y-waveform.h"
#include "y-echo.h"
#include <iostream>
using namespace std;


double g_now;
double g_nextTime;
int g_prog = 0;

XMutex g_mutex;


//-----------------------------------------------------------------------------
// name: audio_callback
// desc: audio callback
//-----------------------------------------------------------------------------
static void audio_callback( SAMPLE * buffer, unsigned int numFrames, void * userData )
{
    // keep track of current time in samples
    g_now += numFrames;
    
    // HACK: rough time keeping for next notes - this logic really should be
    // somewhere else: e.g., in its own class and not directly in the audio callback!
    if( g_now > g_nextTime )
    {
        // lock (to protect vector)
        g_mutex.acquire();
        // move down the vector
        // if( g_noteIndex < g_notes.size() )
//         {
//             // temporary note pointer
//             Note * n = &g_notes[g_noteIndex];
//             // note on!
//             g_synth->noteOn( n->channel, n->pitch, n->velocity * 120 );
//             // HACK: with a major 3rd above!
//             g_synth->noteOn( n->channel, n->pitch + 4, n->velocity * 80 );
//             // check to see next time
//             g_nextTime += n->duration * THEREMAX_SRATE;
//             // move to next note for next time
//             g_noteIndex++;
//         }
        // release lock
        g_mutex.release();
    }
    
    // sum
    SAMPLE sum = 0;
    // num channels
    unsigned int channels = Globals::lastAudioBufferChannels;
    
    // zero out
    memset( Globals::lastAudioBuffer, 0,
           sizeof(SAMPLE)*Globals::lastAudioBufferFrames*channels );
    memset( Globals::lastAudioBufferMono, 0,
           sizeof(SAMPLE)*Globals::lastAudioBufferFrames );
    
    // copy to global buffer
    memcpy( Globals::lastAudioBuffer, buffer,
           sizeof(SAMPLE)*numFrames*channels );
    
    // copy to mono buffer
    for( int i = 0; i < numFrames; i++ )
    {
        // zero out
        sum = 0;
        // loop over channels
        for( int j = 0; j < channels; j++ )
        {
            // sum
            sum += buffer[i*channels + j];
        }
        // set
        Globals::lastAudioBufferMono[i] = sum / channels;
    }
    
    // window it for taper in visuals
    for( int i = 0; i < numFrames; i++ )
    {
        // multiply
        Globals::lastAudioBufferMono[i] *= Globals::audioBufferWindow[i];
    }

}




//-----------------------------------------------------------------------------
// name: theremax_audio_init()
// desc: initialize audio system
//-----------------------------------------------------------------------------
bool theremax_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels )
{
    // initialize
    if( !XAudioIO::init( 1, 1, srate, frameSize, channels, audio_callback, NULL ) )
    {
        // done
        return false;
    }
    
    // allocate
    Globals::lastAudioBuffer = new SAMPLE[frameSize*channels];
    // allocate mono buffer
    Globals::lastAudioBufferMono = new SAMPLE[frameSize];
    // allocate window buffer
    Globals::audioBufferWindow = new SAMPLE[frameSize];
    // set frame size (could have changed in XAudioIO::init())
    Globals::lastAudioBufferFrames = frameSize;
    // set num channels
    Globals::lastAudioBufferChannels = channels;
    
    // compute the window
    hanning( Globals::audioBufferWindow, frameSize );
    
    // // create waveform
    // Globals::waveform = new YWaveform();
    // // place it
    // Globals::waveform->loc.y = 1.5f;
    // // set the width
    // Globals::waveform->setWidth( 2.5f );
    // // set the height
    // Globals::waveform->setHeight( .75f );
    // // initialize it
    // Globals::waveform->init( frameSize );
    // // active?
    // Globals::waveform->active = true;
    
    // add to sim
    // Globals::sim->root().addChild( Globals::waveform );
    
    return true;
}




//-----------------------------------------------------------------------------
// name: vq_audio_start()
// desc: start audio system
//-----------------------------------------------------------------------------
bool theremax_audio_start()
{
    // start the audio
    if( !XAudioIO::start() )
    {
        // done
        return false;
    }
    
    return true;
}
