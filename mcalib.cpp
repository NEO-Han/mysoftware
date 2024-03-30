#include"mcalib.h"

Mcalib::Mcalib()
{
	fx = 1025.6674208968147;
	fy = 1024.5145526178596;
	u0 = 978.0357894730001;
	v0 = 584.7817486984733;
	k1 = 0.17110482659294046;
	k2 = -0.24871937059617658;
	p1 = 0.005612068525458217;
	p2 = 0.012251590506456227;
	k3 = 0.13403909236172828;
	cameraMatirx = (cv::Mat_<double>(3, 3) << fx, 0, u0, 0, fy, v0, 0, 0, 1);
	distCoeffs = (cv::Mat_<double>(1, 5) << k1, k2, p1, p2, k3);
	fx1 = 1081.0927159400962;
	fy1 = 1078.0543910571148;
	u01 = 905.6893228913989;
	v01 = 509.5734695301995;
	k11 = 0.13931733833705703;
	k21 = -0.12405279473832098;
	p11 = -0.010457056230199201;
	p21 = -0.0019984723250644106;
	k31 = -0.16075168409851484;
	cameraMatirx1 = (cv::Mat_<double>(3, 3) << fx1, 0, u01, 0, fy1, v01, 0, 0, 1);
	distCoeffs1 = (cv::Mat_<double>(1, 5) << k11, k21, p11, p21, k31);
}

Mcalib::~Mcalib()
{
}
cv::Mat Mcalib::m_undistort(const cv::Mat& inputImage, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs)
{
    cv::Mat undistortedImage;
    cv::undistort(inputImage, undistortedImage, cameraMatrix, distCoeffs);
    return undistortedImage;
}