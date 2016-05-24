#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "opencv2/imgproc/imgproc.hpp"
#include <QMainWindow>
using namespace cv;


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnDetect_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
