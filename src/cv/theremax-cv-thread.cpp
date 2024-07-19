//-----------------------------------------------------------------------------
// name: theremax-cv-thread.cpp
// desc: cv stuff
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------

#include "theremax-cv-thread.h"

TheremaxCV* theremaxCV;
XThread* xthread;

void *threadfunc(void *parm)
{
    while(true)
    {
        theremaxCV->process();
    }
    return NULL;
}

bool theremax_cv_thread_init( )
{
    cerr << "[theremax]: intializaing computer vision systems..." << endl;
    theremaxCV = new TheremaxCV();
    return theremaxCV->init();
}

bool theremax_cv_thread_start( )
{
    theremaxCV->process();
    xthread = new XThread();
    xthread->start(threadfunc);
    return 1;
};

bool theremax_cv_thread_stop ()
{
    xthread->clear();
    delete xthread;
    return 1;
};
