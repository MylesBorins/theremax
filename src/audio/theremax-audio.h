//-----------------------------------------------------------------------------
// name: theremax-audio.h
// desc: audio stuff
//
// author: Myles Borins (mborins@ccrma.stanford.edu)
//   date: Fall 2013
//-----------------------------------------------------------------------------

#ifndef __THEREMAX_AUDIO_H__
#define __THEREMAX_AUDIO_H__

#include "theremax-audio.h"
#include "theremax-globals.h"
#include "x-thread.h"
#include "y-fft.h"
#include "Reverb.h"
#include <iostream>
using namespace std;


// init audio
bool theremax_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start audio
bool theremax_audio_start();

#endif
