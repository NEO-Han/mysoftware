#pragma once
//#define WIN32_LEAN_AND_MEAN
#include <iostream>
//#include <windows.h>
//#include<winnt.h>
//#include <winuser.h>
#include <Kinect.h>
#include <opencv2/opencv.hpp>

class KinectStereoCamera {
public:
    KinectStereoCamera();

    ~KinectStereoCamera();

    void Initialize();

    void Close();
    cv::Mat GetColorImageFromSensor(IKinectSensor* sensor);
    cv::Mat GetLeftColorImage();
    cv::Mat GetRightColorImage();

private:
    IKinectSensor* sensor1;
    IKinectSensor* sensor2;
};

