//-----------------------------------------------------------------------------
// name: theremax-cv.cpp
// desc: cv stuff
//
// author: Myles Borins
//   date: 2013
// 
// some inspiration from
//
// http://s-ln.in/2013/04/18/hand-tracking-and-gesture-detection-opencv/
// and
// https://github.com/jujojujo2003/OpenCVHandGuesture/blob/master/main.cpp
//-----------------------------------------------------------------------------
#include "theremax-cv.h"

#define CENTER_MAX 1000
#define CENTER_MIN 0

int backgroundFrame=500;

double alpha = 0.9;

//This function returns the square of the euclidean distance between 2 points.
double dist(Point x,Point y)
{
    return (x.x-y.x)*(x.x-y.x)+(x.y-y.y)*(x.y-y.y);
}

//This function returns the radius and the center of the circle given 3 points
//If a circle cannot be formed , it returns a zero radius circle centered at (0,0)
pair<Point,double> circleFromPoints(Point p1, Point p2, Point p3)
{
    double offset = pow((double)p2.x, (double)2) + pow((double)p2.y, (double)2);
    double bc = ( pow((double)p1.x, (double)2) + pow((double)p1.y, (double)2) - offset ) /2.0;
    double cd = (offset - pow((double)p3.x, (double)2) - pow((double)p3.y, (double)2)) /2.0;
    double det = (p1.x - p2.x) * (p2.y - p3.y) - (p2.x - p3.x)* (p1.y - p2.y);
    double TOL = 0.0000001;
    if (abs(det) < TOL) { return make_pair(Point(0,0),0); }

    double idet = 1/det;
    double centerx = (bc * (p2.y - p3.y) - cd * (p1.y - p2.y)) * idet;
    double centery = (cd * (p1.x - p2.x) - bc * (p2.x - p3.x)) * idet;
    double radius = sqrt( pow(p2.x - centerx,2) + pow(p2.y-centery,2));

    return make_pair(Point(centerx,centery),radius);
}

TheremaxCV::~TheremaxCV()
{
    delete camStream;
    cvDestroyWindow(CAMERA_OUTPUT_WINDOW_NAME);
}


bool TheremaxCV::init()
    
{   
    // open a camera stream 
    camStream = new cv::VideoCapture(CV_CAP_ANY);
    
    // oh noes?
    if(!camStream->isOpened())
    {
        cout << "Cannot open cam" << endl;
        return -1;
    }

    // setup background subtractor
    bg.set("nmixtures",3);
    bg.set("detectShadows",false);
    
    filteredPalmCenter.x = 0;
    filteredPalmCenter.y = 0;
    
    return true;
};

void TheremaxCV::process()
{
    vector<vector<Point> > contours;
    
    // get the frame
    camStream->read(this->frame);
    
    if ( !frame.empty() )
    {
        if ( backgroundFrame > 0)
        {
            bg.operator ()(frame,fore);
            backgroundFrame--;
        }
        else
        {
            bg.operator () (frame, fore, 0);
        }

        //Get background image to display it
        bg.getBackgroundImage(back);
    
        //Enhance edges in the foreground by applying erosion and dilation
        erode(fore,fore,Mat());
        dilate(fore,fore,Mat());

        //Find the contours in the foreground
        findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        
        for ( int i = 0; i < contours.size(); i++ )
        {
            //Ignore all small insignificant areas
            if(contourArea(contours[i])>=5000)                
            {
                // Draw contour
                vector<vector<Point> > tcontours;
                tcontours.push_back(contours[i]);
                drawContours(frame, tcontours, -1, cv::Scalar(0,0,255), 2);

                // Detect Hull in current contour
                vector<vector<Point> > hulls(1);
                vector<vector<int> > hullsI(1);
                convexHull(Mat(tcontours[0]), hulls[0], false);
                convexHull(Mat(tcontours[0]), hullsI[0], false);
                drawContours(frame, hulls, -1, cv::Scalar(0,255,0), 2);

                // find minimum area retangle to enclose hand
                RotatedRect rect = minAreaRect(Mat(tcontours[0]));
                

                // Find convex defects
                vector<Vec4i> defects;
                if ( hullsI[0].size() > 0 )
                {
                    Point2f rectPoints[4];
                    rect.points( rectPoints );
                    
                    for ( int j = 0; j < 4; j++ )
                    {
                        line( frame, rectPoints[j], rectPoints[(j+1)%4], Scalar(255,0,0), 1, 8 );
                    }
                    
                    Point roughPalmCenter;
                    convexityDefects(tcontours[0], hullsI[0], defects);
                    
                    if( defects.size() >= 3 )
                    {
                        vector<Point> palmPoints;
                        for ( int j = 0; j < defects.size(); j++ )
                        {
                            int startidx = defects[j][0];
                            int endidx = defects[j][1];
                            int faridx = defects[j][2];
                            
                            Point ptStart( tcontours[0][startidx] );
                            Point ptEnd( tcontours[0][endidx] );
                            Point ptFar( tcontours[0][faridx] );
                            
                            // sum up all the hull and defect points to compute average
                            roughPalmCenter += ptFar + ptStart + ptEnd;
                            palmPoints.push_back(ptFar);
                            palmPoints.push_back(ptStart);
                            palmPoints.push_back(ptEnd);
                        }
                        // get the palm center by 1st getting the average of all defect points
                        // this is the rough palm center
                        // Then you choose the closest 3 points and get the circle radius and center formed 
                        // from them which is the palm center
                        roughPalmCenter.x /= defects.size() * 3;
                        roughPalmCenter.y /= defects.size() * 3;
                        
                        Point closestPt = palmPoints[0];
                        vector<pair<double, int> > distvec;
                        
                        for (int k = 0; k < palmPoints.size(); k++)
                        {
                            distvec.push_back( make_pair( dist( roughPalmCenter, palmPoints[k] ), i ) );
                        }
                        sort(distvec.begin(), distvec.end());
                        
                        // keep chosing 3 points till you find a circle with a valid radius
                        // as there is a high chance that the closest points might be in a linear line or too close that it forms a very large circle
                        
                        pair<Point, double> solnCircle;
                        
                        for (int k = 0; (k + 2) < distvec.size(); k++)
                        {
                            Point p0 = palmPoints[distvec[k+0].second];
                            Point p1 = palmPoints[distvec[k+1].second];
                            Point p2 = palmPoints[distvec[k+2].second];
                            
                            solnCircle = circleFromPoints( p0, p1 ,p2 );
                            if ( solnCircle.second != 0 )
                            {
                                break;
                            }
                        }
                        
                        palmCenters.push_back(solnCircle);
                        if( palmCenters.size() > 10 )
                        {
                            palmCenters.erase(palmCenters.begin());
                        }
                        
                        Point palmCenter;
                        double radius = 0;
                        for (int k = 0; k < palmCenters.size(); k++)
                        {
                            palmCenter += palmCenters[i].first;
                            radius += palmCenters[i].second;
                        }
                        palmCenter.x /= palmCenters.size();
                        palmCenter.y /= palmCenters.size();
                        radius /= palmCenters.size();
                        
                        if (palmCenter.x > CENTER_MAX)
                        {
                            palmCenter.x = CENTER_MAX;
                        }
                        else if (palmCenter.x < CENTER_MIN)
                        {
                            palmCenter.x = CENTER_MIN;
                        }
                        
                        if (palmCenter.y > CENTER_MAX)
                        {
                            palmCenter.y = CENTER_MAX;
                        }
                        else if (palmCenter.y < CENTER_MIN)
                        {
                            palmCenter.y = CENTER_MIN;
                        }
                        
                        filteredPalmCenter.x = (alpha * filteredPalmCenter.x) + ((1 - alpha) * palmCenter.x);
                        filteredPalmCenter.y = (alpha * filteredPalmCenter.y) + ((1 - alpha) * palmCenter.y);
                        
                        
                        cerr << filteredPalmCenter << endl;
                    }
                }
            }
        }
        
        // cerr << contours.size() << endl;
    }
    
    



    waitKey(30);
}

// void TheremaxCV::detectHand( Mat frame )
// {
//     // if (!background.empty() )
//     // {
//     //     background = alpha * background + (1 - alpha) * frame;
//     // }
// 
//     
//     vector<Rect> hands;
//     Mat frameGray;
//     
//     cvtColor( frame, frameGray, CV_BGR2GRAY );
//     equalizeHist( frameGray, frameGray );
//     
//     handCascade.detectMultiScale( 
//         frame,
//         hands,
//         1.1,
//         2,
//         0 | CV_HAAR_SCALE_IMAGE,
//         Size(30, 30)
//     );
//         
//     cerr << hands.size() << endl;
// }
// 
// void TheremaxCV::updateGlobals( )
// {
//     if (Globals::cvIntensity > 0.50)
//     {
//         Globals::reverb->fcheckbox0 = false;
//     }
//     else 
//     {
//         Globals::reverb->fcheckbox0 = true;
//     }
//     
//     // Update the reverb ... first the room dimentions
//     double exponent = (Globals::cvIntensity * -1) + 1;
//     exponent = (exponent * -8) + 1;
//     if (exponent > 1)
//     {
//         exponent = 1;
//     }
//     // Room Dimensions / min acoustic ray length
//     Globals::reverb->fhslider1 = pow(10, exponent);
//     // Room Dimensions / max acoustic ray length
//     Globals::reverb->fhslider0 = pow(10, exponent);
//     
//     double tuning = 1500 + 700 * Globals::cvIntensity;
//     // Now some of the upper edges
//     Globals::reverb->fhslider6 = tuning;
//     Globals::reverb->fhslider5 = tuning - 200;
//     
//     Globals::freq = Globals::freq * 0.8;
//     Globals::freq += pow((Globals::cvIntensity * 110 * 0.2), 2);
// }