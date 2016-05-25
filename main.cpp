#include "objectDetection.h"
#include "recognition.h"

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include<iostream>
#include<vector>
#include<string>
#include<stdio.h>

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
const string DATABASE_DIR = "/home/standby/Projects/faceRecognition/faces/";
const string EXT = ".png";
string Name;
const float DETECTION_THRESHOLD = 0.7f;

//function prototypes
void initCamera(VideoCapture &capture);
void detectAndRecognise(VideoCapture &capture, CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade, CascadeClassifier &eyeGlassCascade);
void storeFaces(Mat &processedFace, vector<Mat>& preProcessedFaces, vector<int>& faceLabels);
void writeImage(Mat &image, string name);

detectObject detection;
recognition faceRecognition;

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
    Ptr<FaceRecognizer> model;
    vector<Mat> preProcessedFaces;
    vector<int> faceLabels;
    Mat referenceFace;
    Mat processedImage;
    Mat frame;
    string databaseImage;

    int identity = -1;
    databaseImage = DATABASE_DIR + Name + EXT;

    //Load the database image for the sleected user
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


    while(true)
    {
        //stream camera image to gui window
        capture >> frame;
        imshow("stream", frame);
        char c = waitKey(20);
        if (c == 27){       //if esc key leave program
            break;
        }else if(c == 13){      //if enter
            writeImage(frame, Name);
        }
        else if(c == 32){ //if spacebar capture frame and run detection program
            Mat userFace;
            userFace = detection.processImage(frame, faceCascade, eyeCascade, eyeGlassCascade);
            if(!userFace.empty()){  //if processing successful
                //imshow("userFace", userFace);
                Mat reconstructedFace = faceRecognition.reconstructFace(model, userFace);   //project to pca space                
                double similarity = faceRecognition.getSimilarity(userFace, reconstructedFace); //compare with stored images
                string output;
                if (similarity < DETECTION_THRESHOLD){
                    identity = model->predict(userFace);
                    output = toString(identity);
                    storeFaces(userFace, preProcessedFaces, faceLabels);
                }else{
                    output = "Unkown";
                }
                cout << "Identity: " << Name << ". Similarity: " << similarity << endl;

            }else{
                cout << "image processing failed" << endl;
            }
        }
    }
    cvDestroyWindow("stream");
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
