#include <iostream>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "theremax-audio.h"
#include "theremax-globals.h"
#include "theremax-cv-thread.h"


using namespace std;
using namespace cv;

#define CAMERA_OUTPUT_WINDOW_NAME "camera-output"



int main(int argc, char **argv)
{
    
    // start real-time audio
    if ( !theremax_audio_init( THEREMAX_SRATE, THEREMAX_FRAMESIZE, THEREMAX_NUMCHANNELS ) )
    {
        // error message
        cerr << "[theremax]: cannot initialize real-time audio I/O.." << endl;
        return -1;
    }
    // if ( !theremax_cv_init( ) )
    // {
    //     cerr << "[theremax]: cannot initialize your camera :( ";
    //     return -1;
    // }
    
    theremax_audio_start();
    theremax_cv_thread_init();
    theremax_cv_thread_start();
    while(1)
    {
        // cerr << Globals::cvIntensity << endl;
        String input;
        cerr << "Want me to stop? [y | n]" << endl;
        cin >> input;
        break;
    }
    
    
    
    return 1;
}