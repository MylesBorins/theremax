#include <iostream>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "theremax-audio.h"
#include "theremax-globals.h"
#include "theremax-cv-thread.h"
#include "x-thread.h"
#include "theremax-gfx.h"


using namespace std;
using namespace cv;

#define CAMERA_OUTPUT_WINDOW_NAME "camera-output"



int main(int argc, const char **argv)
{
    theremax_gfx_init(argc, argv);
    // start real-time audio
    if ( !theremax_audio_init( THEREMAX_SRATE, THEREMAX_FRAMESIZE, THEREMAX_NUMCHANNELS ) )
    {
        // error message
        cerr << "[theremax]: cannot initialize real-time audio I/O.." << endl;
        return -1;
    }
    
    // Open webcam and initialize opencv stuffs
    if (!theremax_cv_thread_init())
    {
        // error message
        cerr << "[theremax]: cannot initialize webcam for computer vision magicks.." << endl;
        return -1;
    }
    
    theremax_cv_thread_start();
    // theremax_audio_start();
    
    theremax_gfx_loop();
    return 1;
}