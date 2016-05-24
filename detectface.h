#ifndef DETECTFACE_H
#define DETECTFACE_H

#include <QObject>
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;

class detectFace : public QObject
{
    Q_OBJECT
public:
    detectFace();
    ~detectFace();
    void openStream();
    void detectLargestObject();
    void initCascades(CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade, CascadeClassifier& eyeGlassesCascade);
    void findObject(Mat &image, CascadeClassifier &cascade, Rect &rect, int scaledWidth = 320);
    void detectEyes(const Mat &face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, Point &leftEye, Point &rightEye);
    void equalizeLeftAndRightHalves(Mat &faceImg);
    Mat emitSignal(Mat& img);

    //Mat frame;


    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade;
    CascadeClassifier eyeGlassesCascade;

    Rect faceRect;


private slots:
    void processImage(Mat &img);
    void storeProcessedFace(Mat &processedFace);

signals:
    void checkFrame(Mat &img);
    void processedFace(Mat &processedFace);


};

#endif // DETECTFACE_H
