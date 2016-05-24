#include "recognition.h"

#include "opencv2/opencv.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core.hpp"
//#include "/home/standby/opencv/opencv-2.4.10/modules/contrib/include/opencv2/contrib/contrib.hpp"
#include "opencv/cv.h"
#include "opencv/cxcore.h"

using namespace cv;
using namespace std;

recognition::recognition()
{
}

recognition::~recognition()
{
}

Ptr<FaceRecognizer> recognition::learnCollectedFaces(const vector<Mat> preprocessedFaces, const vector<int> faceLabels, const string facerecAlgorithm)
{
    Ptr<FaceRecognizer> model;

    cout << "Learning faces using: " << facerecAlgorithm << " algorith" << endl;

    //ensure contrib is loaded at runtime
    bool haveContribModule = initModule_contrib();
    if(!haveContribModule){
        cerr << "contrib load failed!" << endl;
        exit(1);
    }

    model = Algorithm::create<FaceRecognizer>(facerecAlgorithm);
    if(model.empty()){
        cerr << "algorithm not available" << endl;
        exit(1);
    }

    //init done, now train from collected faces
    model->train(preprocessedFaces, faceLabels);

    return model;
}

//genereate a reconstructed face by backprojecting eigenvectors and eigenvalues of given preprocessed face
Mat recognition::reconstructFace(const Ptr<FaceRecognizer> model, const Mat preprocessedFace)
{
    try{
        //get required data
        Mat eigenvectors = model->get<Mat>("eigenvectors");
        Mat averageFaceRow = model->get<Mat>("mean");

        int faceHeight = preprocessedFace.rows;

        //project input into PCA subspace
        Mat projection = subspaceProject(eigenvectors, averageFaceRow, preprocessedFace.reshape(1,1));

        //generate reconstructed face back form pca
        Mat reconstructionRow = subspaceReconstruct(eigenvectors, averageFaceRow, projection);

        //convert float row matrix to a regular 8-bit image
        //make rectangular
        Mat reconstructionMat = reconstructionRow.reshape(1, faceHeight);
        //convert to floating point pixels
        Mat reconstructedFace = Mat(reconstructionMat.size(), CV_8U);
        reconstructionMat.convertTo(reconstructedFace, CV_8U, 1, 0);

        return reconstructedFace;
    } catch(cv::Exception e){
        cout << "error: " << endl;
        return Mat();
    }
}

//compare 2 images by getting the L2 error; (sqrt of sum of squared error)
double recognition::getSimilarity(const Mat A, const Mat B)
{
    if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols){
        //Calculate the L2 relative error
        double errorL2 = norm(A,B,CV_L2);
        //convert to reasonable scale
        double similarity = errorL2/ (double)(A.rows * A.cols);
        return similarity;
    }
    else{
        cout << "images have diff size" << endl;
        return 100000000.0;
    }
}
