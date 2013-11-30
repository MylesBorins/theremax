#include <iostream>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

#define CAMERA_OUTPUT_WINDOW_NAME "camera-output"

int main(int argc, char **argv)
{
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
            imshow(CAMERA_OUTPUT_WINDOW_NAME, cameraFrame);
        if (waitKey(30) == 27) {
            cout << "Input" << endl;
            break;
        }
    }

    cout << "Done" << endl;

    cvDestroyWindow(CAMERA_OUTPUT_WINDOW_NAME);
    return 1;
}