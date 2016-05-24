#include "detectface.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include "opencv/cv.h"
#include "opencv/cxcore.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

//#include <gst/gst.h>

using namespace cv;
using namespace std;


const char *faceCascadeFilename = "/home/standby/opencv/opencv-2.4.10/data/lbpcascades/lbpcascade_frontalface.xml";     // LBP face detector.
//const char *eyeCascadeFilename1 = "/home/standby/opencv/opencv-2.4.10/data/haarcascades/haarcascade_mcs_lefteye.xml";               // Basic eye detector for open eyes only.
const char *eyeCascadeFilename1 = "/home/standby/opencv/opencv-2.4.10/data/haarcascades/haarcascade_eye.xml";               // Basic eye detector for open eyes only.

const char *eyeCascadeFilename2 = "/home/standby/opencv/opencv-2.4.10/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.

const int faceWidth = 70;
const double DESIRED_LEFT_EYE_X = 0.16;     // Controls how much of the face is visible after preprocessing.
const double DESIRED_LEFT_EYE_Y = 0.14;
const double FACE_ELLIPSE_CY = 0.40;
const double FACE_ELLIPSE_W = 0.50;         // Should be atleast 0.5
const double FACE_ELLIPSE_H = 0.80;         // Controls how tall the face mask is.

bool faceFound = FALSE;

detectFace::detectFace()
{
    connect(this, SIGNAL(checkFrame(Mat&)), this, SLOT(processImage(Mat&)));
    connect(this, SIGNAL(processedFace(Mat&)), this, SLOT(storeProcessedFace(Mat&)));
}

detectFace::~detectFace()
{
}

Mat storedFace;
Mat detectFace::emitSignal(Mat& img)
{
    //imshow("im", img);
    emit checkFrame(img);
    img.release();

    if(!storedFace.empty()){
        return storedFace;
    }else{
        return Mat();
    }
}

void detectFace::storeProcessedFace(Mat &processedFace)
{
    processedFace.copyTo(storedFace);
    imshow("stored", storedFace);

}

void detectFace::processImage(Mat &img)
{
    Mat greyImage;

    switch(img.channels()){
    case 3:
        cvtColor(img,greyImage, CV_BGR2GRAY);
        break;
    case 4:
        cvtColor(img, greyImage, CV_BGRA2GRAY);
        break;
    default:
        img.copyTo(greyImage);
        break;
    }
    equalizeHist(greyImage, greyImage);     //equalise image
    //imshow("eq", greyImage);
    initCascades(faceCascade, eyeCascade, eyeGlassesCascade);   //initialise cascade classifiers
    //Rect faceRect;
    Mat faceImage;
    findObject(greyImage, faceCascade, faceRect); //search for largest object
    if (faceRect.width > 0){                    //if found
        faceImage = greyImage(faceRect);
        Point leftEye, rightEye;
        detectEyes(faceImage, eyeCascade, eyeGlassesCascade, leftEye, rightEye);
        //imshow("rect",faceImage);
    }else{
        cout << "no face found" << endl;
        faceImage.data = NULL;
        return;
    }
}


void detectFace::initCascades(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade, CascadeClassifier &eyeGlassCascade)
{
    //Load face cascade
    try{
        faceCascade.load(faceCascadeFilename);
    } catch(cv::Exception){}
    if(faceCascade.empty()){
        cout << "Could not load cascade file: " << faceCascadeFilename << endl;
        return;
    }

    //load basic eye cascade
    try{
        eyeCascade.load(eyeCascadeFilename1);
    }catch(cv::Exception){}
    if(eyeCascade.empty()){
        cout << "Could not load cascade file: " << eyeCascadeFilename1 << endl;
        return;
    }

    //load eye glasses cascade
    try{
        eyeGlassCascade.load(eyeCascadeFilename2);
    }catch(cv::Exception){}
    if(eyeGlassCascade.empty()){
        cout << "Could not load cascade file: " << eyeCascadeFilename2 << endl;
        return;
    }
}

void detectFace::findObject(Mat &image, CascadeClassifier &cascade, Rect &rect, int scaledWidth)
{
    int flags = CASCADE_FIND_BIGGEST_OBJECT; //search for 1 large object
    Size minSize = Size(20,20);
    float searchDetailFactor = 1.1f; //higher no. = more strict search, must be > 1.0
    int minNeighbours = 4;  //detection filter. 2 = good+bad, 6=good but some missed, 4 is decent average
    vector<Rect> objects;
    Mat srchImage;

    float scale = image.cols / (float)scaledWidth;
    if(image.cols > scaledWidth){
        //shrink image while keeping aspect ratio
        int scaledHeight = cvRound(image.rows/scale);
        resize(image, srchImage, Size(scaledWidth, scaledHeight));
        //imshow("scaled", srchImage);
    }else{
        srchImage = image;
    }

    cascade.detectMultiScale(srchImage, objects, searchDetailFactor, minNeighbours, flags, minSize);

    // Enlarge the results if the image was temporarily shrunk before detection.
    if (image.cols > scaledWidth) {
        for (int i = 0; i < (int)objects.size(); i++ ) {
            objects[i].x = cvRound(objects[i].x * scale);
            objects[i].y = cvRound(objects[i].y * scale);
            objects[i].width = cvRound(objects[i].width * scale);
            objects[i].height = cvRound(objects[i].height * scale);
        }
    }

    // Make sure the object is completely within the image, in case it was on a border.
    for (int i = 0; i < (int)objects.size(); i++ ) {
        if (objects[i].x < 0)
            objects[i].x = 0;
        if (objects[i].y < 0)
            objects[i].y = 0;
        if (objects[i].x + objects[i].width > image.cols)
            objects[i].x = image.cols - objects[i].width;
        if (objects[i].y + objects[i].height > image.rows)
            objects[i].y = image.rows - objects[i].height;
    }

    if(objects.size()>0){
        rect = (Rect)objects.at(0);    //return largest object
    }else{
        rect = Rect(-1,-1,-1,-1);       //return invalid
    }
}

void detectFace::detectEyes(const Mat &face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2,
                                Point &leftEye, Point &rightEye)
{
    //default values for eye.xml & eyeglasses.xml
    const float EYE_XPOS = 0.16f;
    const float EYE_YPOS = 0.26f;
    const float EYE_WIDTH = 0.30f;
    const float EYE_HEIGHT = 0.28f;

    int leftX = cvRound(face.cols * EYE_XPOS);
    int topY = cvRound(face.rows * EYE_YPOS);
    int widthX = cvRound(face.cols * EYE_WIDTH);
    int heightY = cvRound(face.rows * EYE_HEIGHT);
    int rightX = cvRound(face.cols * (1 - EYE_XPOS - EYE_WIDTH));

    Mat topLeftFace = face(Rect(leftX, topY, widthX, heightY));
    Mat topRightFace = face(Rect(rightX, topY, widthX, heightY));

    Rect leftEyeRect, rightEyeRect;
    findObject(topLeftFace, eyeCascade1, leftEyeRect, topLeftFace.cols);
    findObject(topRightFace, eyeCascade1, rightEyeRect, topRightFace.cols);

    if (leftEyeRect.width > 0) {   // Check if the eye was detected.
        leftEyeRect.x += leftX;    // Adjust the left-eye rectangle because the face border was removed.
        leftEyeRect.y += topY;
        leftEye = Point(leftEyeRect.x + leftEyeRect.width/2, leftEyeRect.y + leftEyeRect.height/2);
    }
    else {
        leftEye = Point(-1, -1);    // Return an invalid point
        cout << "badleft" << endl;
        return;
    }

    if (rightEyeRect.width > 0) { // Check if the eye was detected.
        rightEyeRect.x += rightX; // Adjust the right-eye rectangle, since it starts on the right side of the image.
        rightEyeRect.y += topY;  // Adjust the right-eye rectangle because the face border was removed.
        rightEye = Point(rightEyeRect.x + rightEyeRect.width/2, rightEyeRect.y + rightEyeRect.height/2);
    }
    else {
        rightEye = Point(-1, -1);    // Return an invalid point
        cout << "badright" << endl;
        return;
    }

    //check got both eyes
    if(leftEye.x >= 0 && rightEye.x >= 0){
        /*normalise image size
          rotate, scale and translate the face so that the 2 eyes line up perfectly with stored image*/
        //get the center between 2 eyes
        Point2f eyesCenter = Point2f((leftEye.x + rightEye.x)*0.5f, (leftEye.y + rightEye.y)*0.5f);
        //cout << "eyeCenter: " << eyesCenter << endl;
        //get the angle between the eyes
        double dy = (rightEye.y - leftEye.y);
        double dx = (rightEye.x - leftEye.x);
        double len = sqrt(dx*dx + dy*dy);
        double angle = atan2(dy, dx) * 180.0/CV_PI; //convert from rad to deg
        //cout << "dy: " << dy << " dx: " << dx << " len: " << len << " angle: " << angle << endl;

        //Experimental measurements show left eye centre should be at roughly (0.19,0.14) of scaled face
        const double DESIRED_RIGHT_EYE_X = (1.0f - DESIRED_LEFT_EYE_X);
        //get the amount of scaling required
        int desiredFaceWidth = faceWidth;
        int desiredFaceHeight = desiredFaceWidth;
        double desiredLen = (DESIRED_RIGHT_EYE_X - DESIRED_LEFT_EYE_X) * desiredFaceWidth;
        double scaleFactor = desiredLen/ len;
        //get the transformation matrix for rotating and scaling
        Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, scaleFactor);
        //shift eyecenter to desired value
        rot_mat.at<double>(0,2) += desiredFaceWidth * 0.5f - eyesCenter.x;
        rot_mat.at<double>(1,2) += desiredFaceHeight * DESIRED_LEFT_EYE_Y - eyesCenter.y;

        //rotate, scale and translate image to desired position
        Mat warped = Mat(desiredFaceHeight, desiredFaceWidth, CV_8U, Scalar(128));//clear an output Mat to default grey
        warpAffine(face, warped, rot_mat, warped.size());

        equalizeLeftAndRightHalves(warped);

        Mat filtered = Mat(warped.size(), CV_8U);
        bilateralFilter(warped, filtered, 0, 20.0, 20.0);;

        //filter out corners of face to focus on middle parts
        Mat mask = Mat(warped.size(), CV_8U, Scalar(0)); //start with empty mask
        Point faceCenter = Point(desiredFaceWidth/2, cvRound(desiredFaceHeight * FACE_ELLIPSE_CY));
        Size size = Size(cvRound(desiredFaceWidth * FACE_ELLIPSE_W), cvRound(desiredFaceHeight * FACE_ELLIPSE_H));
        ellipse(mask, faceCenter, size, 0, 0, 360, Scalar(255), CV_FILLED);

        //use the mask to remove outside pixels
        Mat dstImg = Mat(warped.size(), CV_8U, Scalar(128));
        filtered.copyTo(dstImg,mask);
        //imshow("dst",dstImg);
        emit processedFace(dstImg );
    }
    else{
        Mat img;
        img.data = NULL;
        emit processedFace(img);
    }
    return;
}

void detectFace::equalizeLeftAndRightHalves(Mat &faceImg)   //seperately equalise left and right halves of face
{
    /*common for uneven light coniditions across face
      equalise left & right separately, then again equalise resultt
      this creates an average across t eentire image*/

    int width = faceImg.cols;
    int height = faceImg.rows;
    Mat wholeFace;
    equalizeHist(faceImg, wholeFace);

    //equalise 2 halves
    int midX = width/2;
    Mat leftSide = faceImg(Rect(0,0,midX,height));
    Mat rightSide = faceImg(Rect(midX, 0, width-midX, height));
    equalizeHist(leftSide, leftSide);
    equalizeHist(rightSide, rightSide);

    //combine two halves and make a smooth tranisition upon edge
    for (int y=0; y < height; y++){
        for (int x=0; x < width; x++){
            int v;
            if (x < width/4){   //left quarter, only uses leftSide
                v = leftSide.at<uchar>(y,x);
            }
            else if(x < width*2/4){   //mid25%: blend left & whole
                int lv = leftSide.at<uchar>(y,x);
                int wv = wholeFace.at<uchar>(y,x);
                //blend more of teh face as it moves further right
                float f = (x - width*1/4)/(float)(width*0.25f);
                v = cvRound((1.0f - f) * lv + (f) * wv);
            }
            else if(x < width*3/4){     //mid right 25%, blend face and rightSide
                int rv = rightSide.at<uchar>(y,x-midX);
                int wv = wholeFace.at<uchar>(y,x);
                //blend more of the right side as it moves furtehr right
                float f = (x - width*2/4) / (float)(width*0.25f);
                v = cvRound((1.0f - f) * wv + (f) * rv);
            }
            else{       //right 25%
                v = rightSide.at<uchar>(y, x-midX);
            }
            faceImg.at<uchar>(y,x) = v;
        }
    }
}
























