#include "PedestrianDetector.h"

PedestrianDetector::PedestrianDetector()
	:mMaxObjectCounter(0), mObjectSize(0), Detector()
{

}

PedestrianDetector::PedestrianDetector(float confThreshold)
	: mMaxObjectCounter(0), mObjectSize(0), Detector(confThreshold)
{

}

PedestrianDetector::~PedestrianDetector()
{

}

bool PedestrianDetector::Create()
{
	bool ret = mEngine.CreateEngine("pedestrian_detection_adas_0002");
	if (ret)
	{
		int inputChannels = 0;
		int outputChannels = 0;
		mEngine.GetInputLayerInfo(inputChannels, mInputWidth, mInputHeight);
		mEngine.GetOuputLayerInfo(outputChannels, mObjectSize, mMaxObjectCounter);

		return true;
	}
	else
	{
		return false;
	}
}

void PedestrianDetector::getObjects(const float* pDetections, float confThreshold, std::vector<Object>& arrObjects)
{
	for (int i = 0; i < mMaxObjectCounter; i++)
	{
		const float* ptr = pDetections + (i * mObjectSize);

		int img_id = (int)(ptr[0]);
		if (img_id < 0)
		{
			break;
		}
		int label = (int)(ptr[1]);
		float conf = ptr[2];
		float x_min = ptr[3];
		float y_min = ptr[4];
		float x_max = ptr[5];
		float y_max = ptr[6];

		if (conf > confThreshold)
		{
			cv::Rect bbox;
			bbox.x = (int)(x_min * mInputWidth);
			bbox.y = (int)(y_min * mInputHeight);
			bbox.width = (int)((x_max - x_min) * mInputWidth);
			bbox.height = (int)((y_max - y_min) * mInputHeight);

			Object obj;
			obj.SetBBox(bbox);
			obj.SetColor(0, 0, 255);
			obj.SetConfidence(conf);

			arrObjects.push_back(obj);
		}
	}
}

int PedestrianDetector::GetDetections(const cv::Mat& img, std::vector<Object>& arrObjects)
{
	arrObjects.clear();

	const float* pResult = (const float*)(mEngine.Infer(img));
	getObjects(pResult, mConfThreshold, arrObjects);

	return arrObjects.size();
}