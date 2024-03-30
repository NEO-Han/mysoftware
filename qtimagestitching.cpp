#include "qtimagestitching.h"
//#include <opencv2/opencv.hpp>
#include "ui_qtimagestitching.h"

QtImageStitching::QtImageStitching(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::QtImageStitching)
{
    ui->setupUi(this);
    timerL = new QTimer(this);
    timerR = new QTimer(this);

    //cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    //p.start(cfg);
    //m_kinect = new KinectStereoCamera;
    connect(ui->pushButtonLeft, &QPushButton::clicked, this, &QtImageStitching::PushLeftButtonClicked, Qt::QueuedConnection);
    connect(ui->pushButtonRight, &QPushButton::clicked, this, &QtImageStitching::PushRightButtonClicked, Qt::QueuedConnection);
    connect(ui->pushButtonStitch, &QPushButton::clicked, this, &QtImageStitching::PushStitchButtonClicked, Qt::QueuedConnection);

 
    connect(timerL, &QTimer::timeout, this, &QtImageStitching::readLeftFrame, Qt::UniqueConnection);
    connect(timerR, &QTimer::timeout, this, &QtImageStitching::readRightFrame, Qt::UniqueConnection);
    imageprocessorThread = new QThread;
    imageprocessor = new ImageProcessing;
    this->imageprocessor->moveToThread(this->imageprocessorThread);

    connect(this, &QtImageStitching::imagetransformL, this->imageprocessor, &ImageProcessing::ProcessLeft, Qt::QueuedConnection);
    connect(this, &QtImageStitching::imagetransformR, this->imageprocessor, &ImageProcessing::ProcessRight, Qt::QueuedConnection);
    connect(this->imageprocessor, &ImageProcessing::imageProcessed, this, &QtImageStitching::updateStitchImage, Qt::QueuedConnection);



  /*  m_calib = new Mcalib;*/
    //connect(timerL, &QTimer::timeout, this, &QtImageStitching::readFrame);
    //connect(timerR, &QTimer::timeout, this, &QtImageStitching::readFrame);

}

QtImageStitching::~QtImageStitching()
{

    capL.release();
    capR.release();
    imageprocessorThread->quit();
    imageprocessorThread->wait();
    //p.stop();
    //delete m_calib;
    delete imageprocessorThread;
    delete timerL;
    delete timerR;
    delete ui;
}



void QtImageStitching::PushLeftButtonClicked()
{
    capL.open(1);
    capR.open(0);

    if (!capL.isOpened() || !capR.isOpened()) {
        qDebug() << "Error: Unable to open left camera";
        ui->plainTextEdit->appendPlainText("Error: Unable to open left camera");
        return;
    }
    //m_kinect->Initialize();
    timerL->start(33);
    timerR->start(33);

  /*  m_kinect->Initialize();*/
    ui->plainTextEdit->appendPlainText("camera 0 and camera 1 have opened!");

}

void QtImageStitching::PushRightButtonClicked()
{
    capL.release();
    capR.release();
    //p.stop();
    timerL->stop();
    timerR->stop();
    ui->plainTextEdit->appendPlainText("camera 0 and camera 1 have closed!");
}
void  QtImageStitching::PushStitchButtonClicked()
{
    imageprocessorThread->start();
    ui->plainTextEdit->appendPlainText("Image Stitching is start, plesae wait");

}
//

void QtImageStitching::readLeftFrame()
{
    cv::Mat frameL;
    capL >> frameL;
    //frameL = m_kinect->GetLeftColorImage();
    

    if (this->pixmapItemL != NULL && this->sceneL != NULL)
    {
        delete this->pixmapItemL;
        delete this->sceneL;
        this->pixmapItemL = nullptr;
        this->sceneL = nullptr;
    }
    if (!frameL.empty()) {
        QImage imgL(frameL.data, frameL.cols, frameL.rows, frameL.step, QImage::Format_RGB888);

        this->pixmapItemL = new QGraphicsPixmapItem(QPixmap::fromImage(imgL.rgbSwapped()));
        this->sceneL = new QGraphicsScene(this);
        this->sceneL->addItem(this->pixmapItemL);
        ui->graphicsViewLeft->setScene(this->sceneL);
        ui->graphicsViewLeft->fitInView(this->sceneL->sceneRect(), Qt::KeepAspectRatio);
        if (!countL)
        {
        /*    frameL = m_calib->m_undistort(frameL,m_calib->cameraMatirx,m_calib->distCoeffs);*/
            emit imagetransformL(frameL);
            countL++;
        }
    }

}
void QtImageStitching::readRightFrame()
{

    cv::Mat frameR;
    capR >> frameR;
    //frameR = m_kinect->GetRightColorImage();
   
    if (pixmapItemR != NULL && sceneR != NULL)
    {
        delete pixmapItemR;
        delete sceneR;
        pixmapItemR = nullptr;
        sceneR = nullptr;
    }


    if (!frameR.empty()) {
        QImage imgR(frameR.data, frameR.cols, frameR.rows, frameR.step, QImage::Format_RGB888);

        pixmapItemR = new QGraphicsPixmapItem(QPixmap::fromImage(imgR.rgbSwapped()));
        sceneR = new QGraphicsScene(this);
        sceneR->addItem(pixmapItemR);
        ui->graphicsViewRight->setScene(sceneR);
        ui->graphicsViewRight->fitInView(sceneR->sceneRect(), Qt::KeepAspectRatio);
        if (!countR)
        {
            /*frameR = m_calib->m_undistort(frameR, m_calib->cameraMatirx1, m_calib->distCoeffs1);*/
            emit imagetransformR(frameR);
            countR++;
        }
    }

    /* rs2::frameset data = p.wait_for_frames();*/
    //if (pixmapItemR != NULL && sceneR != NULL)
    //{
    //    delete pixmapItemR;
    //    delete sceneR;
    //    pixmapItemR = nullptr;
    //    sceneR = nullptr;
    //}
    //rs2::frameset data = p.wait_for_frames();


    //if (data) {
    //    rs2::frame color = data.get_color_frame();
    //    const int w = color.as<rs2::video_frame>().get_width();
    //    const int h = color.as<rs2::video_frame>().get_height();
    //    cv::Mat image(cv::Size(640, 480), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
    //    QImage img(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    //    pixmapItemR = new QGraphicsPixmapItem(QPixmap::fromImage(img.rgbSwapped()));
    //    sceneR = new QGraphicsScene(this);
    //    sceneR->addItem(pixmapItemR);
    //    ui->graphicsViewRight->setScene(sceneR);
    //    ui->graphicsViewRight->fitInView(sceneR->sceneRect(), Qt::KeepAspectRatio);
    //}
}

//void QtImageStitching::LoadOnnxStitch(const string superPonitPath, const string lighGulePath,const float  matchthresh)
//{
//   std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
//   std::wstring sp = converter.from_bytes(superPonitPath);
//   std::wstring lh = converter.from_bytes(lighGulePath);
//
//   //stitching
//   cv::Mat pano;
//   Ptr<Stitcher> stitcher = Stitcher::create(mode);
//   Ptr<SuperPoint> superpointp = makePtr<SuperPoint>(sp);
//   Ptr<LightGlue> lightglue = makePtr<LightGlue>(lh, mode, matchthresh);
//   stitcher->setPanoConfidenceThresh(0.1f);
//   stitcher->setFeaturesFinder(superpointp);//SpuerPoint feature extraction
//   stitcher->setFeaturesMatcher(lightglue);//LightGlue feature matching
// 
//}

void  QtImageStitching::updateStitchImage(const cv::Mat& image)
{
    if (image.empty())
    {
        qDebug() << "Invalid image!";
        ui->plainTextEdit->appendPlainText("no input imagevector!");
        return;
    }
    

    if (this->pixmapItemS != NULL && this->sceneS != NULL)
    {
        delete this->pixmapItemS;
        delete this->sceneS;
        this->pixmapItemS = nullptr;
        this->sceneS = nullptr;
    }
    /*qDebug() << "Image Width" << image.width();*/
    QImage img(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    this->pixmapItemS = new QGraphicsPixmapItem(QPixmap::fromImage(img.rgbSwapped()));
    this->sceneS = new QGraphicsScene(this);
    this->sceneS->addItem(pixmapItemS);
    ui->graphicsViewStitch->setScene(this->sceneS);
    ui->graphicsViewStitch->fitInView(this->sceneS->sceneRect(), Qt::KeepAspectRatio);
    ui->plainTextEdit->appendPlainText("Image Stitching is successed!");
    this->countL = 0;
    this->countR = 0;
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    std::stringstream filename;
    filename << "output_" << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-'
        << now->tm_mday << '_' << now->tm_hour << '-' << now->tm_min << '-'
        << now->tm_sec << ".jpg";
    cv::imwrite(filename.str(),image);
    /*   if (pixmapItemS != NULL && sceneS != NULL)
   {
       delete pixmapItemS;
       delete sceneS;
       pixmapItemS = nullptr;
       sceneS = nullptr;
   }*/
}


//const QImage& QtImageStitching::imagetransformL(const QImage& imgL) 
//{
//    return imgL;
//}

ImageProcessing::ImageProcessing(QObject* parent)
    : QObject(parent)
{
    mode = cv::Stitcher::SCANS;
    timerS = new QTimer(this);
    timerS->start(33);
    superPointPath = "./pth/superpoint.onnx";
    lightGluePath = "./pth/superpoint_lightglue.onnx";
    matchthresh = 0.25f;
    std::wstring sp = converter.from_bytes(superPointPath);
    std::wstring lh = converter.from_bytes(lightGluePath);
    stitcher = cv::Stitcher::create(mode);
    superpointp = cv::makePtr<SuperPoint>(sp);
    lightglue = cv::makePtr<LightGlue>(lh, mode, matchthresh);
    //sift = cv::makePtr<cv::SIFT>(cv::SIFT::create());
    //sift = cv::SIFT::create();
    //matches = cv::BFMatcher::create(cv::NORM_L2);
    //matches = cv::makePtr<cv::detail::FeaturesMatcher>(cv::BFMatcher::create(cv::NORM_L2));
    stitcher->setPanoConfidenceThresh(0.1f);
    stitcher->setFeaturesFinder(superpointp);//SpuerPoint feature extraction
    stitcher->setFeaturesMatcher(lightglue);//LightGlue feature matching
    connect(timerS, &QTimer::timeout, this, &ImageProcessing::imageOnnx);

}

ImageProcessing::~ImageProcessing()
{
    delete timerS;
}


void ImageProcessing::ProcessLeft(const  cv::Mat& imageL)
{
    this->imageL = imageL;
    this->imageVector.push_back(this->imageL);
    //if (timerS->isActive())
    //{

    //}
    //else
    //{
    //    timerS->start(30);
    //}
}

void ImageProcessing::ProcessRight(const  cv::Mat& imageR)
{
    this->imageR = imageR;
    this->imageVector.push_back(imageR);
    //if (timerS->isActive())
    //{
    //}
    //else
    //{
    //    timerS->start(30);
    //}
}

void ImageProcessing::imageOnnx()
{
   /* timerS->stop();*/
    if (this->imageL.empty() && this->imageR.empty())
    {
        qDebug() << "invalid imageL and imageR!";
        return;
    }
    else if (this->imageL.empty() && !this->imageR.empty())
    {
        qDebug() << "invalid imageL! only imageR is valid!";
        emit imageProcessed(this->imageR);
     /*   return;*/
    }
    else if (!this->imageL.empty() && this->imageR.empty())
    {
        qDebug() << "invalid imageR! only imageL is valid!";
        emit imageProcessed(this->imageL);
    
    }
    else if (this->imageVector.size() > 1&& !this->imageL.empty()&& !this->imageR.empty())
    {

        //stitching
        QElapsedTimer timer;
        timer.start();
    /*    cv::Stitcher::Status status = stitcher->stitch(this->imageVector, this->imageS);*/
   /*     sift->detectAndCompute(this->imageL, cv::noArray(), this->keypoints1, this->descriptors1);
        sift->detectAndCompute(this->imageR, cv::noArray(), this->keypoints2, this->descriptors2);*/
        //std::vector<cv::DMatch> matcher;
        //this->matches->match(descriptors1, descriptors2, matcher);
        status = stitcher->stitch(this->imageVector, this->imageS);
        cout << "status=  " << status << endl;
        if (status == Stitcher::OK)
        {
            emit imageProcessed(imageS);
            qint64 timeTaken = timer.nsecsElapsed();
            qDebug()<<"Stitch is done! and "<<timeTaken/ 1000000.0<<"ms";

        }
        else
        {
            qDebug() << "fail to send imageS";
            return;
        
        }
        this->imageVector.clear();
       
       /* timerS->start(33);*/
    }
 

    // // ģ��ͼ����

    // std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    // std::wstring sp = converter.from_bytes(superPointPath);
    // std::wstring lh = converter.from_bytes(lightGluePath);
    // 
    // //stitching
    // cv::Mat pano;
    // Ptr<Stitcher> stitcher = Stitcher::create(mode);
    // Ptr<SuperPoint> superpointp = makePtr<SuperPoint>(sp);
    // Ptr<LightGlue> lightglue = makePtr<LightGlue>(lh, mode, matchthresh);
    // stitcher->setPanoConfidenceThresh(0.1f);
    // stitcher->setFeaturesFinder(superpointp);//SpuerPoint feature extraction
    // stitcher->setFeaturesMatcher(lightglue);//LightGlue feature matching
    ///* Stitcher::Status status = stitcher->stitch(imgs, pano);*/
    // cv::Mat image = cv::imread("image.jpg");
    // if (!image.empty()) {
    //     // ����ͼ��������
    //     cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    //     // ���������ͼ���͵����߳���ʾ
    //     emit imageProcessed(image);
    // }

}