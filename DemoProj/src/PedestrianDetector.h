#ifndef _PEDESTRIAN_DETECTOR_H_
#define _PEDESTRIAN_DETECTOR_H_

#include "Detector.h"
#include "OVEngine.h"

class PedestrianDetector : public Detector
{
private:
	OVEngine mEngine;
	int mMaxObjectCounter;
	int mObjectSize;

	void getObjects(const float* pDetections, float confThreshold, std::vector<Object>& arrObjects);
public:
	PedestrianDetector();
	PedestrianDetector(float confThreshold);
	~PedestrianDetector();

	bool Create() override;
	int GetDetections(const cv::Mat& img, std::vector<Object>& arrObjects) override;

};

#endif	//_PEDESTRIAN_DETECTOR_H_