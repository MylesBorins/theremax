//-----------------------------------------------------------------------------
// name: theremax-cv.cpp
// desc: cv stuff
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------
#include "theremax-cv.h"

const string cascadePath = "data/haarcascades/palm.xml";

CascadeClassifier handCascade;


TheremaxCV::~TheremaxCV()
{
    delete camStream;
    cvDestroyWindow(CAMERA_OUTPUT_WINDOW_NAME);
}


bool TheremaxCV::init()
    
{    
    camStream = new cv::VideoCapture(CV_CAP_ANY);

    if(!camStream->isOpened())
    {
        cout << "Cannot open cam" << endl;
        return -1;
    }
    
    if( !handCascade.load( cascadePath ) )
    { 
        cerr << "Oh shit" << endl;
        return -1;
    }

    return true;
};

void TheremaxCV::process()
{
    Mat cameraFrame, copy;
    
    camStream->read(cameraFrame);
    
    if ( !cameraFrame.empty( ) )
    {
        this->detectHand( cameraFrame );
    }
    
    waitKey(30);
}

void TheremaxCV::detectHand( Mat frame )
{
    vector<Rect> hands;
    Mat frameGray;
    
    cvtColor( frame, frameGray, CV_BGR2GRAY );
    equalizeHist( frameGray, frameGray );
    
    handCascade.detectMultiScale( 
        frameGray,
        hands,
        1.1,
        2,
        0 | CV_HAAR_SCALE_IMAGE,
        Size(30, 30)
    );
        
    cerr << hands.size() << endl;
}

void TheremaxCV::updateGlobals( )
{
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
    
    Globals::freq = Globals::freq * 0.8;
    Globals::freq += pow((Globals::cvIntensity * 110 * 0.2), 2);
}