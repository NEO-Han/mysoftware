#include"superpoint.h"


SuperPoint::SuperPoint(std::wstring modelPath):m_env(ORT_LOGGING_LEVEL_WARNING, "SuperPoint")
{
	this->m_modelPath = modelPath;
	m_sessionOptions.SetIntraOpNumThreads(2);
	m_sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
	m_extractorSession = new Ort::Session(m_env, modelPath.c_str(), m_sessionOptions);
	m_memoryInfo = new Ort::MemoryInfo(Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU));

}
SuperPoint::~SuperPoint()
{
	if (m_memoryInfo != nullptr)
	{
		delete m_memoryInfo;
		m_memoryInfo = nullptr;
	}
	if (m_extractorSession != nullptr)
	{
		delete m_extractorSession;
		m_extractorSession = nullptr;
	}
}
vector<float> SuperPoint::ApplyTransform(const Mat& image, float& mean, float& std)
{
	Mat floatImage;
	image.convertTo(floatImage, CV_32FC1);

	vector<float> imgData;
	for (int h = 0; h < image.rows; h++)
	{
		for (int w = 0; w < image.cols; w++)
		{
			imgData.push_back(floatImage.at<float>(h, w) / 255.0f);
		}
	}
	return imgData;
	//Mat floatImage;
	//image.convertTo(floatImage, CV_32FC1, 1.0 / 255.0);
	//vector<float> imgData(floatImage.begin<float>(), floatImage.end<float>());
	//return imgData;
}
void SuperPoint::detectAndCompute(InputArray image, InputArray mask,
	std::vector<KeyPoint>& keypoints,
	OutputArray descriptors,
	bool useProvidedKeypoints)
{
	Mat img = image.getMat();
	Mat grayImg;
	float mean, std;
	cvtColor(img, grayImg, COLOR_BGR2GRAY);
	vector<float> imgData = ApplyTransform(grayImg, mean, std);

	vector<int64_t> inputShape{ 1, 1, grayImg.rows, grayImg.cols };
	Ort::Value inputTensor = Ort::Value::CreateTensor<float>(*m_memoryInfo, imgData.data(), imgData.size(), inputShape.data(), inputShape.size());
	const char* input_names[] = { "image" };
	const char* output_names[] = { "keypoints","scores","descriptors" };

	vector<Ort::Value> outputs = m_extractorSession->Run(this->run_options, input_names, &inputTensor, 1, output_names, 3);

	std::vector<int64_t> kpshape = outputs[0].GetTensorTypeAndShapeInfo().GetShape();
	int64* kp = (int64*)outputs[0].GetTensorMutableData<void>();
	int keypntcounts = kpshape[1];
	keypoints.resize(keypntcounts);
	for (int i = 0; i < keypntcounts; i++)
	{
		KeyPoint p;
		int index = i * 2;
		p.pt.x = (float)kp[index];
		p.pt.y = (float)kp[index + 1];
		keypoints[i] = p;
	}

	std::vector<int64_t> desshape = outputs[2].GetTensorTypeAndShapeInfo().GetShape();
	float* des = (float*)outputs[2].GetTensorMutableData<void>();

	Mat desmat = descriptors.getMat();
	desmat.create(Size(desshape[2], desshape[1]), CV_32FC1);
	for (int h = 0; h < desshape[1]; h++)
	{
		for (int w = 0; w < desshape[2]; w++)
		{
			int index = h * desshape[2] + w;
			desmat.at<float>(h, w) = des[index];
		}
	}
	desmat.copyTo(descriptors);

}
void SuperPoint::detect(InputArray image,
	std::vector<KeyPoint>& keypoints,
	InputArray mask)
{
	Mat img = image.getMat();
	Mat grayImg;
	float mean, std;
	cvtColor(img, grayImg, COLOR_BGR2GRAY);
	vector<float> imgData = ApplyTransform(grayImg, mean, std);
	vector<int64_t> inputShape{ 1, 1, grayImg.rows, grayImg.cols };
	//Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
	Ort::Value inputTensor = Ort::Value::CreateTensor<float>(*m_memoryInfo, imgData.data(), imgData.size(), inputShape.data(), inputShape.size());

	const char* input_names[] = { "image" };
	const char* output_names[] = { "keypoints","scores","descriptors" };

	vector<Ort::Value> outputs = m_extractorSession->Run(this->run_options, input_names, &inputTensor, 1, output_names, 3);

	std::vector<int64_t> kpshape = outputs[0].GetTensorTypeAndShapeInfo().GetShape();
	int64* kp = (int64*)outputs[0].GetTensorMutableData<void>();
	int keypntcounts = kpshape[1];
	keypoints.resize(keypntcounts);
	for (int i = 0; i < keypntcounts; i++)
	{
		KeyPoint p;
		int index = i * 2;
		p.pt.x = (float)kp[index];
		p.pt.y = (float)kp[index + 1];
		keypoints[i] = p;
	}
}
void SuperPoint::compute(InputArray image,
	std::vector<KeyPoint>& keypoints,
	OutputArray descriptors)
{
	Mat img = image.getMat();
	Mat grayImg;
	float mean, std;
	cvtColor(img, grayImg, COLOR_BGR2GRAY);
	vector<float> imgData = ApplyTransform(grayImg, mean, std);

	vector<int64_t> inputShape{ 1, 1, grayImg.rows, grayImg.cols };
	Ort::Value inputTensor = Ort::Value::CreateTensor<float>(*m_memoryInfo, imgData.data(), imgData.size(), inputShape.data(), inputShape.size());

	const char* input_names[] = { "image" };
	const char* output_names[] = { "keypoints","scores","descriptors" };
	vector<Ort::Value> outputs = m_extractorSession->Run(this->run_options, input_names, &inputTensor, 1, output_names, 3);

	std::vector<int64_t> kpshape = outputs[0].GetTensorTypeAndShapeInfo().GetShape();
	int64* kp = (int64*)outputs[0].GetTensorMutableData<void>();
	int keypntcounts = kpshape[1];
	keypoints.resize(keypntcounts);
	for (int i = 0; i < keypntcounts; i++)
	{
		KeyPoint p;
		int index = i * 2;
		p.pt.x = (float)kp[index];
		p.pt.y = (float)kp[index + 1];
		keypoints[i] = p;
	}

	std::vector<int64_t> desshape = outputs[2].GetTensorTypeAndShapeInfo().GetShape();
	float* des = (float*)outputs[2].GetTensorMutableData<void>();
	Mat desmat = descriptors.getMat();
	desmat.create(Size(desshape[2], desshape[1]), CV_32FC1);
	for (int h = 0; h < desshape[1]; h++)
	{
		for (int w = 0; w < desshape[2]; w++)
		{
			int index = h * desshape[2] + w;
			desmat.at<float>(h, w) = des[index];
		}
	}
	desmat.copyTo(descriptors);
}
