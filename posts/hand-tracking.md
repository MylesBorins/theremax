{
  title: "hand tracking",
  date:   "2013-08-3",
  description: "openCV"
}

<br>
![Hand Tracking](/images/hand-tracking.png)
<br>

---

>###Follow the birdy
>The project began with a very naive algorithm for tracking hands.  While I managed to research and attempt to implement a number of different algorithms, I was unsuccessful at finding an approach that gave results as reliable as my initial algorithm.

---

###Average Brightness

My original algorithm for tracking the hand was to calculate the average brightness.  This was done by converting the rgb color space image captured from the camera to an HSV color space.  This HSV image was then fed to a _getBrightness function that calculated luminosity based on certain coefficients.  I found an implementation of this function in the cache of the open cv forum at [this address](http://answers.opencv.org/answers/24342/revisions/)

```c++
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

    //-- percentage conversion factor
    brightness = summ[0]/((::pow(2,8)-1)*frame.rows * frame.cols) * 2; 
}
```

I used a simple low pass filter to clean up the incoming data, opting to take a power of the new incoming data to increase the sensitivity

```c++
double alpha = 0.5
Globals::cvIntensity =
    (alpha * Globals::cvIntensity) + ((1 - alpha) * pow(brightness, 3));
```

This number was then scaled in various ways (with some magic numbers) to change a number of controls on the FDN filter that is used to produce the audio of the application.

Fairly quickly I found that the expression of this algorithm was fairly limited, and that if I wanted to really emulate the experience of playing a theremin I would need to implement a computer vision algorithm that would give me exact coordinates as opposed to average brightness.

### Haar Cascade Classifiers

My first attempt to implement hand tracking was to use a HAAR Cascade classifier trained to recognize hands.  I decided to attempt this method after seeing how quickly Haar Cascades could be used to track faces within a camera frame.  This code was written by modifying the openCV example for face tracking.

```c++
void TheremaxCV::detectHand( Mat frame )
{    
    vector<Rect> hands;
    Mat frameGray;
    
    cvtColor( frame, frameGray, CV_BGR2GRAY );
    equalizeHist( frameGray, frameGray );
    
    handCascade.detectMultiScale( 
        frame,
        hands,
        1.1,
        2,
        0 | CV_HAAR_SCALE_IMAGE,
        Size(30, 30)
    );
        
    cerr << hands.size() << endl;
}
```

Unfortunately no matter what haarcascade I used, and what modifications I made to the detectMultiScale function I could not get a reliable read of hands due to many false positives.

###Convex Points

The next approach I attempted to implement was a combination of a number of algorithms including Gaussian Mixture-based Background/Foreground Segmentation and The Sklanskys algorithm in order to detect convex points.

This was based on [this paper](http://sanjayslnarayanan.files.wordpress.com/2013/08/handtrackingopencv.pdf), the source code can be found [here](https://github.com/jujojujo2003/OpenCVHandGuesture)

While I managed to get the algorithm compiling and running I was unable to get reliable results similar to those shown in the original report.

```c++
vector<vector<Point> > contours;

// get the frame
camStream->read(this->frame);

if ( !frame.empty() )
{
    if ( backgroundFrame > 0)
    {
        bg.operator () (frame,fore);
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
}
```

Above is the code used to find the initial contours of the image, these contours are then used to find hands / fingers.

### Round and back again
In the end I decided to move back to my original algorithm.  It would appear that doing hand tracking on a stock webcam is less than desirable if one is not in perfect conditions.  There was one last algorithm I was researching using adaptive histograms and the camshift algorithm, [sample code](https://github.com/blr246/adaptive-histogram-camshift), although I did not have time to attempt to implement it.