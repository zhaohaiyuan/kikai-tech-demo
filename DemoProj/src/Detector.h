#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include <vector>
#include <opencv2\opencv.hpp>
#include "Object.h"
#include "config.h"

class Detector
{
protected:
	int mInputWidth;
	int mInputHeight;
	float mConfThreshold;

public:
	Detector();
	Detector(float confThreshold);
	virtual ~Detector();

	virtual bool Create() = 0;
	virtual int GetDetections(const cv::Mat& img, std::vector<Object>& arrObjects) = 0;

	void SetConfidenceThreshold(float confThreshold);
	int GetCoreWidth() const;
	int GetCoreHeight() const;

	static Detector* Create(const std::string& name);
};

#endif	//_DETECTOR_H_S