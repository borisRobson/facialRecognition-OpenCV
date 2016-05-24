#include "objectDetection.h"
#include "recognition.h"

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv/cv.h"
#include "opencv/cxcore.h"

#include<iostream>
#include<vector>
#include<string>
#include<stdio.h>

using namespace cv;
using namespace std;

template <typename T> string toString(T t)
{
    ostringstream out;
    out << t;
    return out.str();
}

//define variables to be used in program
const int CAMERA_WIDTH = 640;
const int CAMERA_HEIGHT = 480;
const string DATABASE_IMAGE = "/home/standby/Projects/faceRecognition/faces/Brandon.png";

//function prototypes
void initCamera(VideoCapture &capture);

void detectAndRecognise(VideoCapture &capture, CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade, CascadeClassifier &eyeGlassCascade);
void storeFaces(Mat &processedFace, vector<Mat>& preProcessedFaces, vector<int>& faceLabels);

const float DETECTION_THRESHOLD = 0.7f;

detectObject detection;
recognition faceRecognition;

int main (int argc, char* argv[])
{
    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade;
    CascadeClassifier eyeGlassCascade;
    VideoCapture capture;

    //initialise the three cascade classifiers to be used
    //initCascades(faceCascade, eyeCascade, eyeGlassCascade);
    detection.initCascades(faceCascade, eyeCascade, eyeGlassCascade);

    //initialise the camera
    initCamera(capture);

    //enter program loop
    detectAndRecognise(capture, faceCascade, eyeCascade, eyeGlassCascade);

    return 0;
}

void initCamera(VideoCapture& capture)
{
    try{
        capture.open(0);
        capture.set(CV_CAP_PROP_FRAME_WIDTH,CAMERA_WIDTH);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
        if(capture.isOpened()){
            cout << "Stream opened sucessfully" << endl;
        }else{
            cout << "Error opening stream" << endl;
        }
    }
    catch(cv::Exception &e){
        cout << "Error: " << endl;
    }
}

void detectAndRecognise(VideoCapture &capture, CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade, CascadeClassifier& eyeGlassCascade)
{
    Ptr<FaceRecognizer> model;
    vector<Mat> preProcessedFaces;
    vector<int> faceLabels;
    Mat referenceFace;
    Mat processedImage;

    int identity = -1;

    //Load the database image for the sleected user
    referenceFace = imread(DATABASE_IMAGE,1);
    if (referenceFace.empty()){
        cout << "Error loading database image - check path: " << DATABASE_IMAGE << endl;
        return;
    }

    //put image through preProcessing - returns a Mat of the face ROI
    processedImage = detection.processImage(referenceFace, faceCascade, eyeCascade, eyeGlassCascade);
    imshow("processed", processedImage);

    //Add the processed face to the array
    //Train the recogniser
    storeFaces(processedImage, preProcessedFaces, faceLabels);
    model = faceRecognition.learnCollectedFaces(preProcessedFaces, faceLabels);

    //free up resources
    processedImage.release();

    Mat frame;
    while(true)
    {
        //stream camera image to gui window
        capture >> frame;
        imshow("stream", frame);
        char c = waitKey(20);
        if (c == 27){       //if esc key leave program
            break;
        } else if(c == 32){ //if spacebar capture frame and run detection program
            Mat userFace;
            userFace = detection.processImage(frame, faceCascade, eyeCascade, eyeGlassCascade);
            if(!userFace.empty()){  //if processing successful
                imshow("userFace", userFace);
                storeFaces(userFace, preProcessedFaces, faceLabels);
                Mat reconstructedFace = faceRecognition.reconstructFace(model, userFace);   //project to pca space
                double similarity = faceRecognition.getSimilarity(userFace, reconstructedFace); //compare with stored images
                string output;
                if (similarity < DETECTION_THRESHOLD){
                    identity = model->predict(userFace);
                    output = toString(identity);
                }else{
                    output = "Unkown";
                }
                cout << "Identity: " << output << ". Similarity: " << similarity << endl;

            }else{
                cout << "image processing failed" << endl;
            }
        }
    }
    cvDestroyWindow("stream");
    return;
}

void storeFaces(Mat &processedFace, vector<Mat>& preProcessedFaces, vector<int>& faceLabels)
{
    Mat mirror;
    flip(processedFace, mirror, 1);
    //flip and store image so that FaceRecognizer has more training data
    preProcessedFaces.push_back(processedFace);
    preProcessedFaces.push_back(mirror);
    //only one user so label is always the same
    faceLabels.push_back(0);
    faceLabels.push_back(0);
    cout << "processed faces: " << preProcessedFaces.size() << endl;
    return;
}


