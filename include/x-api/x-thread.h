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
// name: x-thread.h
// desc: thread library
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//          Jieun Oh (jieun5@ccrma.stanford.edu)
//          adapted from ChucK programming language
//          adapted from Synthesis Toolkit
//    date: Fall 2010
//-----------------------------------------------------------------------------
#ifndef __MCD_X_THREAD_H__
#define __MCD_X_THREAD_H__

#include "x-def.h"




#if ( defined(__PLATFORM_MACOSX__) || defined(__PLATFORM_LINUX__) || defined(__WINDOWS_PTHREAD__) )
  #include <pthread.h>
  #define THREAD_TYPE
  typedef pthread_t THREAD_HANDLE;
  typedef void * THREAD_RETURN;
  typedef void * (*THREAD_FUNCTION)(void *);
  typedef pthread_mutex_t MUTEX;
  #define CHUCK_THREAD pthread_t
#elif defined(__PLATFORM_WIN32__)
  #include <windows.h>
  #include <process.h>
  #define THREAD_TYPE __stdcall
  typedef unsigned long THREAD_HANDLE;
  typedef unsigned THREAD_RETURN;
  typedef unsigned (__stdcall *THREAD_FUNCTION)(void *);
  typedef CRITICAL_SECTION MUTEX;
  #define CHUCK_THREAD HANDLE
#endif




//-----------------------------------------------------------------------------
// name: struct XThread
// desc: ...
//-----------------------------------------------------------------------------
struct XThread
{
public:
    XThread();
    ~XThread();

public:
    // begin execution of the thread routine
    // the thread routine can be passed an argument via ptr
    bool start( THREAD_FUNCTION routine, void * ptr = NULL );

    // wait the specified number of milliseconds for the thread to terminate
    bool wait( long milliseconds = -1 );

public:
    // test for a thread cancellation request.
    static void test( );
    
    // clear
    void clear() { thread = 0; }

protected:
    THREAD_HANDLE thread;
};




//-----------------------------------------------------------------------------
// name: struct XMutex
// desc: ...
//-----------------------------------------------------------------------------
struct XMutex
{
public:
    XMutex();
    ~XMutex();

public:
    void acquire( );
    void release(void);

protected:
    MUTEX mutex;
};




#endif
