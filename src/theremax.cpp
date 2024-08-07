//-----------------------------------------------------------------------------
// name: theremax.cpp
// desc: theremin to the max
//
// author: Myles Borins
//   date: fall 2013
//
//-----------------------------------------------------------------------------
#include <iostream>
#include "theremax-audio.h"
#include "theremax-cv-thread.h"
#include "theremax-gfx.h"

using namespace std;

int main(int argc, const char **argv)
{
    unsigned int inputDevice = 0;
    unsigned int outputDevice = 1;

    // check variable for input / output devices
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--list-devices") == 0) {
            theremax_audio_list_devices();
            return 0;
        } else if (strcmp(argv[i], "--input-device") == 0 && i + 1 < argc) {
            inputDevice = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--output-device") == 0 && i + 1 < argc) {
            outputDevice = atoi(argv[++i]);
        }
    }

    // Initialize graphics engine / simulation
    if ( !theremax_gfx_init(argc, argv) )
    {
        cerr << "[theremax]: cannot initialize graphics.... derp" << endl;
        return -1;
    }
    // Open webcam and initialize opencv stuffs
    if (!theremax_cv_thread_init())
    {
        // error message
        cerr << "[theremax]: cannot initialize webcam for computer vision magicks.." << endl;
        return -1;
    }

    // initialize real-time audio
    if ( !theremax_audio_init( THEREMAX_SRATE, THEREMAX_FRAMESIZE, THEREMAX_NUMCHANNELS, inputDevice, outputDevice ) )
    {
        // error message
        cerr << "[theremax]: cannot initialize real-time audio I/O.." << endl;
        return -1;
    }
    
    // Lets get this shop on the road!!!
    theremax_cv_thread_start();
    theremax_audio_start();
    theremax_gfx_loop();
    return 1;
}