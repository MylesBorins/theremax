//-----------------------------------------------------------------------------
// name: theremax-cv.h
// desc: cv stuff
//
// author: Myles Borins
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __THEREMAX_CV_H__
#define __THEREMAX_CV_H__

//http://docs.opencv.org/modules/imgproc/doc/histograms.html?highlight=histogram#CvHistogram*%20cvCreateHist%28int%20dims,%20int*%20sizes,%20int%20type,%20float**%20ranges,%20int%20uniform%29
#include <iostream>
#include <fstream>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "theremax-globals.h"
#include "x-thread.h"
#include "x-fun.h"

using namespace std;
using namespace cv;

#define CAMERA_OUTPUT_WINDOW_NAME "camera-output"

class TheremaxCV
{
public:
    ~TheremaxCV();
public:
    bool init();
    void getIntensity();
    void process();
public:
    cv::VideoCapture * camStream;
    cv::CascadeClassifier cascadeClassifier;
};

#endif
