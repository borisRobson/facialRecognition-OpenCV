#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "detectface.h"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

detectFace faceDecector;
using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnDetect_clicked()
{
    //faceDecector.emitSignal(1);
    Mat face = imread("/home/standby//Projects/faceRecognition/faces/stock.png",1);
    Mat processed;
    if (!face.empty()){
        processed = faceDecector.emitSignal(face);
    }
    else{
        cout << "imread fail" << endl;
    }
}
