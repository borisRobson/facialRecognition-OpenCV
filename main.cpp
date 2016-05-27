#include "objectDetection.h"
#include "recognition.h"
#include "captureimages.h"

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include<iostream>
#include<vector>
#include<string>
#include<stdio.h>
#include <stdexcept>
#include <QTimer>

using namespace cv;
using namespace std;

//function to print useful data
template <typename T> string toString(T t)
{
    ostringstream out;
    out << t;
    return out.str();
}

//define variables to be used in program
const int CAMERA_WIDTH = 640;
const int CAMERA_HEIGHT = 480;
#ifdef IMX6
const string DATABASE_DIR = "/nvdata/config/faces/";
#else
const string DATABASE_DIR = "/home/standby/Projects/faceRecognition/faces/";
#endif
const string EXT = ".png";
string Name;
const float DETECTION_THRESHOLD = 0.7f;
const int TIMEOUT = 75;
const int matchThreshold = 5;
const int ESC_KEY = 27;
const int ENTER_KEY = 13;
const int SPACE_KEY = 32;

//function prototypes
void initCamera(VideoCapture &capture);
void detectAndRecognise(VideoCapture &capture, CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade, CascadeClassifier &eyeGlassCascade);
void storeFaces(Mat &processedFace, vector<Mat>& preProcessedFaces, vector<int>& faceLabels);
void writeImage(Mat &image, string name);


detectObject detection;
recognition faceRecognition;
captureImages captureImage;

/*
  Program entry point - initialises cascades and camera
  then enters program loop
*/
int main (int argc, char* argv[])
{
    if (argc > 0){
        Name = argv[1];
    }

    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade;
    CascadeClassifier eyeGlassCascade;
    VideoCapture capture;

    //initialise the three cascade classifiers to be used
    //initCascades(faceCascade, eyeCascade, eyeGlassCascade);
    detection.initCascades(faceCascade, eyeCascade, eyeGlassCascade);

    //initialise the camera
    initCamera(capture);

    //check if user exists
    //enter program loop
    detectAndRecognise(capture, faceCascade, eyeCascade, eyeGlassCascade);

    return 0;
}

/*
    Initialises and opens camera stream
    @params VideoCapture
*/
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

/*
    Main program loop
    Loads database image, processess it and then trains the FaceRecogniser
    Streams camera image, on button press captures frame, processess and compares
    @params VideoCapture; FaceCascade; eyeCascade; eyeGlassCascade
*/

void detectAndRecognise(VideoCapture &capture, CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade, CascadeClassifier& eyeGlassCascade)
{
    cout << "Face Recognition with open cv" << endl;
    cout << "press 'spacebar' to capture image and compare with database" << endl;
    cout << "press 'enter' to add user" << endl;
    cout << "press 'esc' to exit" << endl;

    vector<Mat> userFaces;
    Ptr<FaceRecognizer> model;
    vector<Mat> preProcessedFaces;
    vector<Mat>capturedAndProcessedFaces;
    vector<int> faceLabels;
    Mat referenceFace;
    Mat processedImage;
    Mat frame;
    Mat userFace;
    string databaseImage;
    int matches = 0;
    double similarity;

    int identity = -1;
    databaseImage = DATABASE_DIR + Name + EXT;

    //Try to load image specified by Name
    //if fail enter loop waiting for enter keypress
    try{
        referenceFace = imread(databaseImage,1);
    }catch(cv::Exception&e){};
    if (referenceFace.empty()){
        cout << "User : " << Name << " not found, press Enter to add photo" << endl;
        while (true){
            capture >> frame;
            imshow("stream", frame);
            char c = waitKey(20);
            if (c == 13){
                writeImage(frame, Name);
                referenceFace = imread(databaseImage, 1);
                break;
            }
        }
    }
    //put image through preProcessing - returns a Mat of the face ROI
    processedImage = detection.processImage(referenceFace, faceCascade, eyeCascade, eyeGlassCascade);
    if(!processedImage.empty()){
        imshow("processed", processedImage);
        //Add the processed face to the array
        //Train the recogniser
        storeFaces(processedImage, preProcessedFaces, faceLabels);
        model = faceRecognition.learnCollectedFaces(preProcessedFaces, faceLabels);
    }
    //free up resources
    processedImage.release();

    bool keyPressed =false;
    while(true)
    {
        //stream camera image to gui window
        capture >> frame;
        imshow("stream", frame);
        if (!captureImage.timer->isActive() && keyPressed ){    //if timer has elapsed and key has been pressed
            captureImage.startTimer(TIMEOUT, capture, userFaces);
            keyPressed = false;
            int size = userFaces.size();
            if(size != 0){      //confirm that array has been filled
                imshow("userface", userFaces.at(9));
                int faceCount = userFaces.size();
                //process each captured image
                for (int i = 0; i < faceCount; i++){
                    Mat face = userFaces.at(i);
                    //imshow(toString(i), face);
                    userFace = detection.processImage(face, faceCascade, eyeCascade, eyeGlassCascade);
                    if(!userFace.empty()){  //if processing successful
                        //imshow("userFace", userFace);
                        capturedAndProcessedFaces.push_back(userFace);
                    }
                }
                //use successfully processed images for face recogniser
                for (int i = 0; i < (int)capturedAndProcessedFaces.size(); i++){
                    Mat reconstructedFace = faceRecognition.reconstructFace(model, userFace);   //project to pca space
                    similarity = faceRecognition.getSimilarity(userFace, reconstructedFace); //compare with stored images
                    string output;
                    if (similarity < DETECTION_THRESHOLD){
                        identity = model->predict(userFace);
                        output = toString(identity);
                        storeFaces(userFace, preProcessedFaces, faceLabels);
                        //model = faceRecognition.learnCollectedFaces(preProcessedFaces, faceLabels); //re-train face rec with more matches
                        matches++;
                    }else{
                        output = "Unknown";
                    }
                }
                if(matches >= matchThreshold){
                     cout << "Identity: " << Name << ". Similarity: " << similarity << " matches: " << matches << endl;
                }else{
                    cout << "insufficient match count: " << matches << endl;
                }
            }else{
                cout << "image processing failed" << endl;
            }
        }

        char c = waitKey(20);
        if (c == ESC_KEY){       //if esc key leave program
            break;
        }else if(c == ENTER_KEY){      //if enter
            writeImage(frame, Name);
        }
        else if(c == SPACE_KEY){ //if spacebar capture frame and run detection program
            keyPressed=true;
            captureImage.startTimer(TIMEOUT, capture, userFaces);
        }
    }    
    cvDestroyAllWindows();
    return;
}

/*
  Adds processed face image to array of faces
  @params processedFace(single image); preProcessedFaces(array); faceLabels(array)
*/

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

/*
    writes image to specified location in png format
    @params - image, name(string);
*/

void writeImage(Mat &image, string name)
{
    //set parameters fopr appropriate file formate
   vector<int> compression_params;
   compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
   compression_params.push_back(9);
   string filename = DATABASE_DIR + name+".png";
    //write image
   try {
       imwrite(filename, image, compression_params);
   }
   catch (cv::Exception &ex) {
       fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
       return;
   }

   fprintf(stdout, "Saved PNG file with alpha data.\n");
   return;
}









