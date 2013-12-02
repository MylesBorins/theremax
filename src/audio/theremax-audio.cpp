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
#include "Reverb.h"
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
    // lock (to protect vector)
    Globals::reverb->compute(numFrames, Globals::finputs, Globals::foutputs);
    
    for (int j = 0; j < THEREMAX_FRAMESIZE; j++)
    {
        for (int i = 0; i < THEREMAX_NUMCHANNELS; i++)
        {
            buffer[(j * THEREMAX_NUMCHANNELS) + i] = Globals::biquad->tick(Globals::foutputs[i][j]);
        }
    }
    
    g_mutex.acquire();
    // cerr << Globals::freq << endl;
    // release lock
    g_mutex.release();
    
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
    
    // create Reverb
    Globals::reverb = new Reverb();
    Globals::reverb->init(THEREMAX_SRATE);
    
    // Setup reverb
    // Band 0 upper edge
    Globals::reverb->fhslider6 = 1500;
    // Band 1 upper edge
    Globals::reverb->fhslider5 = 1000;
    // Band 2 upper edge
    Globals::reverb->fhslider4 = 135;
    // Band 3 upper edge
    Globals::reverb->fhslider3 = 344;

    // Band 0 t60
    Globals::reverb->fvslider3 = 3.0f;
    // Band 1 t60
    Globals::reverb->fvslider4 = 10.0f;
    // Band 2 t60
    Globals::reverb->fvslider2 = 3.0f;
    // Band 3 t60
    Globals::reverb->fvslider1 = 10.0f;
    // Band 4 t60
    Globals::reverb->fvslider0 = 3.0f;


    // Room Dimensions / min acoustic ray length
    Globals::reverb->fhslider1 = 10.0f;
    // Room Dimensions / max acoustic ray length
    Globals::reverb->fhslider0 = 10.0f;
    
    // Mute external input
    Globals::reverb->fcheckbox1 = true;
    // Add pink noise
    Globals::reverb->fcheckbox0 = true;
    
    for (int i = 0; i < THEREMAX_NUMCHANNELS; i++)
    {
        Globals::finputs[i] = new FAUSTFLOAT[THEREMAX_FRAMESIZE];
        memset( Globals::finputs[i], 0,
               sizeof(FAUSTFLOAT)*THEREMAX_FRAMESIZE );
        Globals::foutputs[i] = new FAUSTFLOAT[THEREMAX_FRAMESIZE];
        memset( Globals::foutputs[i], 0,
               sizeof(FAUSTFLOAT)*THEREMAX_FRAMESIZE );
    }
    
    // Setup Biquad
    Globals::biquad = new stk::BiQuad;
    Globals::biquad->setCoefficients(
        0.0009405043311967682,
        0.0018810086623935365,
        0.0009405043311967682,
        -1.9113981953542545,
        0.9151602126790416
    );
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
