//http://docs.opencv.org/modules/imgproc/doc/histograms.html?highlight=histogram#CvHistogram*%20cvCreateHist%28int%20dims,%20int*%20sizes,%20int%20type,%20float**%20ranges,%20int%20uniform%29
#include <iostream>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "theremax-audio.h"
#include "theremax-globals.h"


using namespace std;
using namespace cv;

#define CAMERA_OUTPUT_WINDOW_NAME "camera-output"

void _getBrightness(const Mat& frame, double& brightness)
{
    Mat temp, color[3], lum;
    temp = frame;

    split(temp, color);

    color[0] = color[0] * 0.299;
    color[1] = color[1] * 0.587;
    color[2] = color[2] * 0.114;


    lum = color[0] + color [1] + color[2];

    Scalar summ = sum(lum);


    brightness = summ[0]/((::pow(2,8)-1)*frame.rows * frame.cols) * 2; //-- percentage conversion factor
 }

int main(int argc, char **argv)
{
    
    // start real-time audio
    if ( !theremax_audio_init( THEREMAX_SRATE, THEREMAX_FRAMESIZE, THEREMAX_NUMCHANNELS ) )
    {
        // error message
        cerr << "[theremax]: cannot initialize real-time audio I/O.." << endl;
        return -1;
    }
    VideoCapture camStream(CV_CAP_ANY);

    if(!camStream.isOpened())
    {
        cout << "Cannot open cam" << endl;
        return -1;
    }
    cout << "Camera opened successfully" << endl;

    cvNamedWindow(CAMERA_OUTPUT_WINDOW_NAME, CV_WINDOW_AUTOSIZE);

    while (true) {
        Mat cameraFrame;
        
        camStream >> cameraFrame;
        if (cameraFrame.dims != 0)
        {
            Mat frameHSV;
            double brightness;
            _getBrightness(cameraFrame, brightness);
            cerr << brightness << endl;
            imshow( CAMERA_OUTPUT_WINDOW_NAME, cameraFrame );
        }
            
        if (waitKey(30) == 27) {
            cout << "Input" << endl;
            break;
        }
    }

    cout << "Done" << endl;
    camStream.release();
    cvDestroyWindow(CAMERA_OUTPUT_WINDOW_NAME);
    return 1;
}