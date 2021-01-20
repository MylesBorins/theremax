//-----------------------------------------------------------------------------
// name: theremax-cv.cpp
// desc: cv stuff
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------
#include "theremax-cv.h"

// set alpha to 0.5 for low pass filter
double alpha = 0.5f;

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
    destroyWindow(CAMERA_OUTPUT_WINDOW_NAME);
}


bool TheremaxCV::init()
    
{    
    camStream = new cv::VideoCapture(CAP_ANY);

    if(!camStream->isOpened())
    {
        cout << "Cannot open cam" << endl;
        return -1;
    }

    return true;
};

void TheremaxCV::process()
{
    Mat cameraFrame, copy;
    
    camStream->read(cameraFrame);
    
    if (!cameraFrame.empty())
    {
        Mat frameHSV;
        double brightness;
        _getBrightness(cameraFrame, brightness);

        // One Pole Lowpass filter
        Globals::cvIntensity = (alpha * Globals::cvIntensity) + ((1 - alpha) * pow(brightness, 3));
        
        if (Globals::cvIntensity > 0.50)
        {
            Globals::reverb->fcheckbox0 = false;
        }
        else 
        {
            Globals::reverb->fcheckbox0 = true;
        }
        
        // Update the reverb ... first the room dimentions
        double exponent = (Globals::cvIntensity * -1) + 1;
        exponent = (exponent * -8) + 1;
        if (exponent > 1)
        {
            exponent = 1;
        }
        // Room Dimensions / min acoustic ray length
        Globals::reverb->fhslider1 = pow(10, exponent);
        // Room Dimensions / max acoustic ray length
        Globals::reverb->fhslider0 = pow(10, exponent);
        
        double tuning = 1500 + 700 * Globals::cvIntensity;
        // Now some of the upper edges
        Globals::reverb->fhslider6 = tuning;
        Globals::reverb->fhslider5 = tuning - 200;
    }
}

