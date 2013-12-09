//-----------------------------------------------------------------------------
// name: theremax-cv.cpp
// desc: cv stuff
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------
#include "theremax-cv.h"

const string cascadePath = "data/haarcascades/palm.xml";

CascadeClassifier handClassifier;

vector<Rect> objects;

double intensityMin = 1;
double intensityMax = 0;

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


// void detect( Mat& img, CascadeClassifier& cascade,
//                     double scale, bool tryflip )
// {
//     int i = 0;
//     double t = 0;
//     vector<Rect> faces, faces2;
//     const static Scalar colors[] =  { CV_RGB(0,0,255),
//         CV_RGB(0,128,255),
//         CV_RGB(0,255,255),
//         CV_RGB(0,255,0),
//         CV_RGB(255,128,0),
//         CV_RGB(255,255,0),
//         CV_RGB(255,0,0),
//         CV_RGB(255,0,255)} ;
//     Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
// 
//     cvtColor( img, gray, COLOR_BGR2GRAY );
//     resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
//     equalizeHist( smallImg, smallImg );
// 
//     t = (double)cvGetTickCount();
//     cascade.detectMultiScale( smallImg, faces,
//         1.1, 2, 0
//         //|CASCADE_FIND_BIGGEST_OBJECT
//         //|CASCADE_DO_ROUGH_SEARCH
//         |CASCADE_SCALE_IMAGE
//         ,
//         Size(30, 30) );
//     if( tryflip )
//     {
//         flip(smallImg, smallImg, 1);
//         cascade.detectMultiScale( smallImg, faces2,
//                                  1.1, 2, 0
//                                  //|CASCADE_FIND_BIGGEST_OBJECT
//                                  //|CASCADE_DO_ROUGH_SEARCH
//                                  |CASCADE_SCALE_IMAGE
//                                  ,
//                                  Size(30, 30) );
//         for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++ )
//         {
//             faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
//         }
//     }
//     t = (double)cvGetTickCount() - t;
//     printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
// }


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
    
    // This seems to be needed to do anything
    
    if( !handClassifier.load( cascadePath ) )
    { 
        cerr << "Oh shit" << endl;
    }
    
    cvNamedWindow(CAMERA_OUTPUT_WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    
    return true;
};

void TheremaxCV::process()
{
    Mat cameraFrame, copy;
    
    camStream->read(cameraFrame);
    // detect( cameraFrame, cascadeClassifier, 1, 0 );
    // cascadeClassifier->detectMultiScale(cameraFrame, objects,
    //     1.1, 2, 0
    //     //|CASCADE_FIND_BIGGEST_OBJECT
    //     //|CASCADE_DO_ROUGH_SEARCH
    //     |CASCADE_SCALE_IMAGE
    //     ,
    //     Size(30, 30) );
//     
//     cerr << objects.size() << endl;
    
    if (cameraFrame.dims != 0)
    {
        Mat frameHSV;
        double brightness;
        _getBrightness(cameraFrame, brightness);
        if (brightness > intensityMax)
        {
            intensityMax = brightness;
        }
        if (brightness < intensityMin)
        {
            intensityMin = brightness;
        }
        // cerr << "pre: " << brightness;
        // cerr << " post: " << brightness << endl;
        Globals::cvIntensity *= 0.5;
        Globals::cvIntensity += pow(brightness, 3) * 0.5;
        
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
    waitKey(30);
}


void TheremaxCV::getIntensity()
{
    Mat cameraFrame;
    camStream->read(cameraFrame);
    Mat frameHSV;
    double brightness;
    _getBrightness(cameraFrame, brightness);
    Globals::cvIntensity *= 0.5;
    Globals::cvIntensity += (brightness * 0.5);
}
