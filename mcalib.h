#pragma once
#include <opencv2/opencv.hpp>
//using namespace cv;
class  Mcalib
{
public:
	 Mcalib();
	~ Mcalib();
	cv::Mat m_undistort(const cv::Mat& inputImage, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs);
	//Mat m_undistort_r(const cv::Mat& inputImage, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs);
private:
	double fx;
	double fy;
	double u0;
	double v0;
	double k1;
	double k2;
	double p1;
	double p2;
	double k3;

	double fx1;
	double fy1;
	double u01;
	double v01;
	double k11;
	double k21;
	double p11;
	double p21;
	double k31;

public:
	cv::Mat cameraMatirx;
	cv::Mat distCoeffs;
	cv::Mat cameraMatirx1;
	cv::Mat distCoeffs1;
};