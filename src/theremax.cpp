//http://docs.opencv.org/modules/imgproc/doc/histograms.html?highlight=histogram#CvHistogram*%20cvCreateHist%28int%20dims,%20int*%20sizes,%20int%20type,%20float**%20ranges,%20int%20uniform%29
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