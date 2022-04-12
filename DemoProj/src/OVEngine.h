#ifndef _OV_ENGINE_H_
#define _OV_ENGINE_H_

#include <string>
#include <opencv2\opencv.hpp>
#include <inference_engine.hpp>

using namespace InferenceEngine;

class OVEngine
{
private:
	Core mCore;
	CNNNetwork mNetwork;
	ExecutableNetwork mExeNetwork;
	InferRequest mInferReq;
	Blob::Ptr mOutputBlob;
	std::string mModelName;
	std::string mDeviceType;
	std::string mInputLayerName;
	std::string mOutputLayerName;
	int mInputChannels;
	int mInputWidth;
	int mInputHeight;
	int mOutputChannels;
	int mOutputWidth;
	int mOutputHeight;

public:
	OVEngine();
	~OVEngine();

	bool CreateEngine(const std::string& modelName);

	/*
	void SetBlob(const std::string& layerName, const Blob::Ptr& blob);
	Blob::Ptr GetBlob(const std::string& layerName);
	*/

	Blob::Ptr GetBlob(const cv::Mat& mat);
	const void* Infer(const cv::Mat& mat);
	const void* Infer(const Blob::Ptr& data);

	void GetInputLayerInfo(int& channels, int& width, int& height);
	void GetOuputLayerInfo(int& channels, int& width, int& height);
};

#endif //_OV_ENGINE_H_