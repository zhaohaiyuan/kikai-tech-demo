#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <vector>
#include <opencv2\opencv.hpp>
#include "Detector.h"
#include "config.h"

struct AppConfig
{
	std::string InputSource;
	float ConfidenceThreshold;
	std::vector<cv::Point> RestrictedArea;

	AppConfig()
	{
		InputSource = "";
		ConfidenceThreshold = DEFAULT_CONFIDENCE_THRESHOLD;
		RestrictedArea.clear();
	}
};

class Application
{
private:
	Detector* mpDetector;
	AppConfig mConfig;
	bool mIsRunning;

	bool isInsidePolygon(const cv::Point2f& point, const std::vector<cv::Point>& polygon);

	void drawRestrictedArea(cv::Mat& img, const cv::Scalar& color);
	
public:
	Application(AppConfig* pConfig);
	~Application();

	void Start();
	//void Stop();
};
#endif //_APPLICATION_H_