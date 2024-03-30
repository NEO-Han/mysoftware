#pragma once
#include <vector>
#include <string>
#include <memory>
#include<iostream>
#include "opencv2/stitching.hpp"
#include "opencv2/calib3d.hpp"
#include "common.h"
#include <onnxruntime_cxx_api.h>

using namespace cv::detail;
using namespace cv;
using namespace std;

class  FEATURE_MATCHER_EXPORTS LightGlue :public FeaturesMatcher
{
protected:
	cv::Stitcher::Mode m_mode;//Affine or Perspective。仿射变换还是透视变换
	std::wstring m_modelPath;
	std::vector<detail::ImageFeatures> features_;
	std::vector<detail::MatchesInfo> pairwise_matches_;
	float m_matchThresh = 0.0;//匹配阈值
	CV_WRAP_AS(apply) void operator ()(const ImageFeatures& features1, const ImageFeatures& features2,
		CV_OUT MatchesInfo& matches_info) {
		match(features1, features2, matches_info);
	}
	void AddFeature(detail::ImageFeatures features);
	void AddMatcheinfo(const MatchesInfo& matches_info);
public:
	Ort::Env m_env;
	Ort::SessionOptions m_sessionOptions;
	Ort::Session* m_matcherSession;
	Ort::MemoryInfo* m_memoryInfo;
	Ort::RunOptions run_options;
	LightGlue(std::wstring modelPath, Stitcher::Mode mode, float matchThresh);
	~LightGlue();
	void match(const ImageFeatures& features1, const ImageFeatures& features2,
		MatchesInfo& matches_info);
	std::vector<detail::ImageFeatures> features() { return features_; };
	std::vector<detail::MatchesInfo> matchinfo() { return pairwise_matches_; };

};
