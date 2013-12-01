#include "theremax-cv.h"


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

TheremaxCV::~TheremaxCV()
{
    delete camStream;
    delete xthread;
}

bool TheremaxCV::init()
{
    camStream = new cv::VideoCapture(CV_CAP_ANY);
    xthread = new XThread;
    if(!camStream->isOpened())
    {
        cout << "Cannot open cam" << endl;
        return -1;
    }
    cout << "Camera opened successfully" << endl;
    return true;
};




    // VideoCapture camStream(CV_CAP_ANY);

    
    // 
    // cvNamedWindow(CAMERA_OUTPUT_WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    // 
    // while (!Globals::dead) {
    //     Mat cameraFrame;
    //     
    //     camStream >> cameraFrame;
    //     if (cameraFrame.dims != 0)
    //     {
    //         Mat frameHSV;
    //         double brightness;
    //         _getBrightness(cameraFrame, brightness);
    //         Globals::cvIntensity *= 0.5;
    //         Globals::cvIntensity += (brightness * 0.5);
    //         imshow( CAMERA_OUTPUT_WINDOW_NAME, cameraFrame );
    //     }
    //     if (waitKey(30) == 27) {
    //         cout << "Input" << endl;
    //         Globals::dead = true;
    //         break;
    //     }
    // }
    // 
    // cout << "Done" << endl;
    // camStream.release();
    // cvDestroyWindow(CAMERA_OUTPUT_WINDOW_NAME);
