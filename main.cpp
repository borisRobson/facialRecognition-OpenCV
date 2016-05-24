#include <QtGui/QApplication>
#include "mainwindow.h"
#include "detectface.h"
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

VideoCapture capture;
Mat frame, face;
Ptr<FaceRecognizer> model;
vector<Mat> preProcessedFaces;
vector<int> faceLabels;

const float DETECTION_THRESHOLD = 0.7f;

void storeFaces(Mat &processedFace);

template <typename T> string toString(T t)
{
    ostringstream out;
    out << t;
    return out.str();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    detectFace faceDetector;
    recognition faceRec;
    MainWindow w;
    //w.show();

    int identity = -1;

    try{

        //Load database images
        Mat refFace = imread("/home/standby//Projects/faceRecognition/faces/Brandon.png",1);
        Mat processed = faceDetector.emitSignal(refFace);

        if(!processed.empty()){
            storeFaces(processed);
            model = faceRec.learnCollectedFaces(preProcessedFaces, faceLabels);
        }
        refFace.release();
        processed.release();

        //init cam
        capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
        //capture.open("v4l2src ! videoscale ! video/x-raw-yuv,width=640,height=480 ! ffmpegcolorspace ! appsink");
        capture.open(0);        //open cam
    } catch(cv::Exception &e){}
    if(!capture.isOpened()){
        cout << "could not open cam" << endl;
        return -1;
    }
    cout << "stream opened successfully " << endl;
    while(1){
        capture >> frame;       //grab frame from cam stream
        imshow("stream",frame);     //and display it
        char c = waitKey(20);
        if(c==27){              //if esc
            break;
        }else if(c==32){        //else if spacebar
           // imshow("frame",frame);
            face = faceDetector.emitSignal(frame);
           // imshow("face", face);
            if(!face.empty()){
                storeFaces(face);
                model = faceRec.learnCollectedFaces(preProcessedFaces, faceLabels);

                Mat reconstructedFace = faceRec.reconstructFace(model, face);
                imshow("reconstructed", reconstructedFace);

                double similarity = faceRec.getSimilarity(face, reconstructedFace);
                string output;
                if (similarity < DETECTION_THRESHOLD){
                    //identify person
                    identity = model->predict(face);
                    output = toString(identity);
                }
                else{
                    output = "Unknown";
                }
                cout << "Identity: " << output << ". Similarity: " << similarity << endl;
            }
        }
    }
    cvDestroyWindow("stream");
    return 0;
}

void storeFaces(Mat& processedFace)
{
    Mat mirror;
    flip(processedFace, mirror, 1);
    //flip and store image so that FaceRecognizer has more training data
    preProcessedFaces.push_back(processedFace);
    preProcessedFaces.push_back(mirror);
    faceLabels.push_back(0);
    faceLabels.push_back(0);
    cout << "processed faces: " << preProcessedFaces.size() << endl;
    return;
}



