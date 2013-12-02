//-----------------------------------------------------------------------------
// name: theremax-cv-thread.h
// desc: cv stuff
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __THEREMAX_CV_THREAD_H__
#define __THEREMAX_CV_THREAD_H__

#include <stdio.h>
#include "theremax-globals.h"
#include "x-thread.h"
#include "theremax-cv.h"

using namespace std;

// init cv
bool theremax_cv_thread_init();
// start cv
bool theremax_cv_thread_start();

#endif
