#include "captureimages.h"

#include <QTimer>
#include <QtCore>
#include <iostream>
#include<stdio.h>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

VideoCapture cap;
vector<Mat>faces;

const int imageCount = 10;

template <typename T> string toString(T t)
{
    ostringstream out;
    out << t;
    return out.str();
}
bool done;

captureImages::captureImages()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(captureImage()));
}

void captureImages::startTimer(int interval, VideoCapture& capture, vector<Mat>& userFaces)
{
    if (!done){
        cap = capture;
        faces= userFaces;
        count  = 0;
        timer->start(interval);
    }else{
        userFaces = faces;
        done = false;
    }
}

void captureImages::captureImage()
{
    Mat face;
    if(count < imageCount){
        cap >> face;
        faces.push_back(face);
    }else{
        done = true;
        timer->stop();
    }
    count++;
}
