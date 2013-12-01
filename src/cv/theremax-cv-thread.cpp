#include "theremax-cv-thread.h"

TheremaxCV theremaxCV;
bool theremax_cv_thread_init( )
{
    theremaxCV.init();
    return 1;
}

bool theremax_cv_thread_start( )
{
    return 1;
};