#pragma once

#include <QtCore/qdebug.h>
#include <QtCore/qtimer.h>
#include <codecvt>
#include <QtCore/qthread.h>
#include <iostream>
#include <QElapsedTimer>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <opencv2/opencv.hpp>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QMainWindow>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include "superpoint.h"
#include "lightglue.h"
#include "common.h"
#include "ui_qtimagestitching.h"
//#include "KinectStereoCamera.h"
#include <ctime>
#include <sstream>


class QTimer;
class VideoCapture;

QT_BEGIN_NAMESPACE
namespace Ui { class QtImageStitching; }
QT_END_NAMESPACE


class ImageProcessing : public QObject
{
    Q_OBJECT
public:
    /* explicit ImageProcessingThread(QObject* parent = nullptr) : QThread(parent) {}*/
    ImageProcessing(QObject* parent = nullptr);
    ~ImageProcessing();
    QTimer* timerS;
    std::vector<cv::Mat> imageVector;
    cv::Mat imageS;//stitch frame
public slots:
    void imageOnnx();
    void ProcessLeft(const  cv::Mat& imageL);
    void ProcessRight(const  cv::Mat& imageR);


private:
    cv::Stitcher::Mode mode;
    cv::Mat imageL;//left frame
    cv::Mat imageR;//right frame
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    string superPointPath;//SuperPoint ONNX format model path
    string lightGluePath;//LightGlue ONNX format model path
    float  matchthresh;
    cv::Ptr<Stitcher> stitcher;
    //std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Stitcher::Status status;
    //cv::Mat descriptors1, descriptors2;
    //cv::Ptr<cv::Feature2D> sift;
    //cv::Ptr<cv::BFMatcher> matches;
    cv::Ptr<SuperPoint> superpointp;
    cv::Ptr<LightGlue> lightglue;

signals:
    void imageProcessed(const  cv::Mat& image);
};


class QtImageStitching : public QMainWindow
{
    Q_OBJECT

public:
    QtImageStitching(QWidget* parent = nullptr);
    ~QtImageStitching();
    std::vector<cv::Mat> imageVector;
    int countL = 0;
    int countR = 0;
    

private slots:
    void PushLeftButtonClicked();
    void PushRightButtonClicked();
    void PushStitchButtonClicked();
    void readLeftFrame();
    void readRightFrame();
    void updateStitchImage(const cv::Mat& image);

private:
    QImage MatToQImage(const cv::Mat& mat);				// MAT类型 转 QImage类型



private:
    Ui::QtImageStitching* ui;


    QTimer* timerL;
    QTimer* timerR;
    //QImage qimgL;
    //QImage qimgR;
    //QImage qimgS;

    cv::VideoCapture capL;
    cv::VideoCapture capR;
    //rs2::pipeline p;
    //rs2::config cfg;
  /*  Mcalib* m_calib;*/
    QThread* imageprocessorThread;
    ImageProcessing* imageprocessor;
    //KinectStereoCamera* m_kinect;

    QGraphicsScene* sceneL = nullptr;
    QGraphicsScene* sceneR = nullptr;
    QGraphicsScene* sceneS = nullptr;
    QGraphicsPixmapItem* pixmapItemL = nullptr;
    QGraphicsPixmapItem* pixmapItemR = nullptr;
    QGraphicsPixmapItem* pixmapItemS = nullptr;

signals:
    void imagetransformL(const cv::Mat& imgL);
    void imagetransformR(const cv::Mat& imgR);
};



