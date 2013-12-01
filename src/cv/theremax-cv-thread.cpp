#include "theremax-cv-thread.h"

TheremaxCV theremaxCV;
XThread thread;

void *threadfunc(void *parm)
{
    while(true)
    {
        theremaxCV.process();
    }
    return NULL;
}

bool theremax_cv_thread_init( )
{
    theremaxCV.init();
    return 1;
}

bool theremax_cv_thread_start( )
{
    theremaxCV.process();
    thread.start(threadfunc);
    return 1;
};