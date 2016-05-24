#ifndef DETECTFACE_H
#define DETECTFACE_H

#include <QObject>
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;

class detectObject : public QObject
{
    Q_OBJECT
public:
    detectObject();
    ~detectObject();
    //void openStream();
    void detectLargestObject();
    void initCascades(CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade, CascadeClassifier& eyeGlassesCascade);
    Rect findObject(Mat &image, CascadeClassifier &cascade, int scaledWidth = 320);
    Mat detectEyes(Mat& face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, Point &leftEye, Point &rightEye);
    void equalizeLeftAndRightHalves(Mat &faceImg);
    Mat processImage(Mat &img, CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade, CascadeClassifier& eyeGlassCascade);
    Mat emitSignal(Mat& img);





    //Rect faceRect;


private slots:
//    Mat processImage(Mat &img);
   // void storeProcessedFace(Mat &processedFace);

signals:
    //void checkFrame(Mat &img);
   // void processedFace(Mat &processedFace);


};

#endif // DETECTFACE_H
