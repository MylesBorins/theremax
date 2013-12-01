//-----------------------------------------------------------------------------
// name: theremax-cv.h
// desc: cv stuff
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __THEREMAX_AUDIO_H__
#define __THEREMAX_AUDIO_H__




// init cv
bool theremax_cv_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start cv
bool theremax_cv_start();

#endif
