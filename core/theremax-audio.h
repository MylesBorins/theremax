//-----------------------------------------------------------------------------
// name: theremax-audio.h
// desc: audio stuff
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __THEREMAX_AUDIO_H__
#define __THEREMAX_AUDIO_H__




// init audio
bool theremax_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start audio
bool theremax_audio_start();

// play some notes
void theremax_playNotes( float pitch, float velocity );


#endif
