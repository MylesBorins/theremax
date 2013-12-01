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



// // globals
// YFluidSynth * g_synth;
// YEcho * g_echo;
double g_now;
double g_nextTime;
int g_prog = 0;




// basic note struct
struct Note
{
    int channel;
    float pitch;
    float velocity;
    float duration; // in seconds
    // add more stuff?

    // constructor
    Note( int c, float p, float v, float d )
    {
        channel = c;
        pitch = p;
        velocity = v;
        duration = d;
    }
};


// HACK: vector of notes
vector<Note> g_notes;
int g_noteIndex = 0;
XMutex g_mutex;




// play some notes
void theremax_playNotes( float pitch, float velocity )
{
    // lock
    g_mutex.acquire();
    // clear notes
    g_notes.clear();
    for( int i = 0; i < 24; i++ )
    {
        // next notes
        g_notes.push_back( Note( 0, pitch + i*2, (1 - i/24.0), .05 + .15*(1 - i/24.0) ) );
    }
    // unlock
    g_mutex.release();

    // reset the index
    g_noteIndex = 0;
    
    // play now!
    g_nextTime = g_now;
}




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

    // set in the wave
    // Globals::waveform->set( Globals::lastAudioBufferMono, numFrames );
    
    // synthesize it
    // g_synth->synthesize2( buffer, numFrames );
    // echo it
    // g_echo->synthesize2( buffer, numFrames );
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
    
    // // instantiate
    // g_synth = new YFluidSynth();
    // // init
    // g_synth->init( srate, 32 );
    // // load the soundfont
    // g_synth->load( "data/sfonts/rocking8m11e.sf2", "" );
    // // map program changes
    // g_synth->programChange( 0, 0 );
    // g_synth->programChange( 1, 79 );
    // g_synth->programChange( 2, 4 );
    // g_synth->programChange( 3, 10 );
    // g_synth->programChange( 4, 13 );
    // 
    // // allocate echo
    // g_echo = new YEcho( srate );
    // g_echo->setDelay( 0, .25 );
    // g_echo->setDelay( 1, .5 );

//    // make a note
//    g_note = makeNote( 0, 60, .9, .5, 0 );
    
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
