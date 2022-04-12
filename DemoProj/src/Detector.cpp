#include "config.h"
#include "Detector.h"
#include "PedestrianDetector.h"

Detector::Detector() 
{
	mConfThreshold = DEFAULT_CONFIDENCE_THRESHOLD;
}

Detector::Detector(float confThreshold)
{
	SetConfidenceThreshold(confThreshold);
}

Detector::~Detector()
{

}

int Detector::GetCoreWidth() const
{
	return mInputWidth;
}

int Detector::GetCoreHeight() const
{
	return mInputHeight;
}

void Detector::SetConfidenceThreshold(float confThreshold)
{
	if (confThreshold < 0)
		confThreshold = 0;
	else if (confThreshold > 1)
		confThreshold = 1;
	mConfThreshold = confThreshold;
}

Detector* Detector::Create(const std::string& name)
{
	if ("pedestrian" == name)
	{
		return new PedestrianDetector();
	}
	return nullptr;
}