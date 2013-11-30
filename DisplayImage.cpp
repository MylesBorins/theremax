#include <iostream>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

#define CAMERA_OUTPUT_WINDOW_NAME "camera-output"

int main(int argc, char **argv)
{
    CvCapture *camCapture;
    int ret = 0;

    if (!(camCapture = cvCaptureFromCAM(CV_CAP_ANY))) {
        cout << "Failed to capture from camera" << endl;

        ret = 1;

        goto exitCameraOpenFailed;
    }

    cout << "Camera opened successfully" << endl;

    cvNamedWindow(CAMERA_OUTPUT_WINDOW_NAME, CV_WINDOW_AUTOSIZE);

    IplImage *cameraFrame;
    int grabFrameRet;

    while (true) {
        if ((cameraFrame = cvQueryFrame(camCapture))) {
            cvShowImage(CAMERA_OUTPUT_WINDOW_NAME, cameraFrame);
        }

        if (cvWaitKey(60) != -1) {
            cout << "Input" << endl;
            break;
        }
    }

    cout << "Done" << endl;

    cvReleaseCapture(&camCapture);
    cvDestroyWindow(CAMERA_OUTPUT_WINDOW_NAME);
exitCameraOpenFailed:
    return ret;
}