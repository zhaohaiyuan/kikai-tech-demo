#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\video.hpp>
#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include "Application.h"

Application::Application(AppConfig* pConfig)
	:mpDetector(nullptr)
{
	mConfig = (*pConfig);
}

Application::~Application()
{
	if (mpDetector != nullptr)
	{
		delete mpDetector;
		mpDetector = nullptr;
	}
}

bool Application::isInsidePolygon(const cv::Point2f& point, const std::vector<cv::Point>& polygon)
{
	int nbVertices = polygon.size();
	if (nbVertices < 3)
	{
		return false;
	}
	double dist = cv::pointPolygonTest(polygon, point, false);
	return (dist > 0);
}

void Application::drawRestrictedArea(cv::Mat& img, const cv::Scalar& color)
{
	cv::Mat layer = cv::Mat::zeros(img.size(), CV_8UC3);
	std::vector<cv::Point>& area = mConfig.RestrictedArea;
	int nbVertices = area.size();
	if (nbVertices > 2)
	{
		cv::polylines(img, area, true, color, 3, CV_AA);
	}
}

void Application::Start()
{
	if (mpDetector != nullptr)
	{
		delete mpDetector; 
		mpDetector = nullptr;
	}
	mpDetector = Detector::Create("pedestrian");

	if (mpDetector == nullptr)
	{
		std::cout << "Can not create the pedestrian-detector" << std::endl;
		return;
	}

	cv::VideoCapture cap(mConfig.InputSource);
	if (!cap.isOpened())
	{
		std::cout << "Can not open video " << mConfig.InputSource << std::endl;
		return;
	}
	
	int frame_id = 0;
	cv::Mat frame;
	cv::Mat review_img;
	cv::Mat core_img;
	std::vector<Object> arr_objects;

	double review_scale = 0.6;
	mpDetector->Create();
	int core_width = mpDetector->GetCoreWidth();
	int core_height = mpDetector->GetCoreHeight();
	std::vector<cv::Point2d> core_restricted_area;
	
	mIsRunning = true;
	while (mIsRunning)
	{
		if (cap.read(frame))
		{
			frame_id++;
			cv::resize(frame, core_img, cv::Size(core_width, core_height));
			double scale_x = (double)frame.cols / core_width;
			double scale_y = (double)frame.rows / core_height;

			std::vector<cv::Point>& area = mConfig.RestrictedArea;

			drawRestrictedArea(frame, cv::Scalar(0, 255, 0));

			int nb_detections = mpDetector->GetDetections(core_img, arr_objects);
			if (nb_detections > 0)
			{
				for (int i = 0; i < nb_detections; i++)
				{
					Object& obj = arr_objects[i];
					obj.Scale(scale_x, scale_y);

					if (isInsidePolygon(obj.GetCenter(), mConfig.RestrictedArea))
					{
						obj.SetColor(255, 0, 0);
						obj.SetTitle("Not permitted");
					}
					obj.DrawTo(frame);
				}
			}
			std::string info_str = "Total Pedestrians: " + std::to_string(nb_detections);
			cv::putText(frame, info_str, cv::Point(10, 40), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 255), 2);
			cv::imshow(APP_NAME, frame);
			int k = cv::waitKey(30);
			switch (k)
			{
			case 27:
				mIsRunning = false;
				break;
			case 'c':
				cv::imwrite("frame_" + std::to_string(frame_id) + ".png", frame);
				break;
			default:
				break;
			}
		}
		else
		{
			mIsRunning = false;
		}
	}

}