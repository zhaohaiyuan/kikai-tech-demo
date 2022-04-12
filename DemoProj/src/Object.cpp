#include <opencv2\imgproc.hpp>
#include "Object.h"

Object::Object()
	: mConfidence(0), mColor(0), mName(""), mTitle("")
{
	Object("", "");
}

Object::Object(const std::string& name)
	: mConfidence(0), mColor(0), mTitle("")
{
	Object("", name);
}

Object::Object(const std::string& title, const std::string& name)
	: mConfidence(0), mColor(0)
{
	mTitle = title;
	mName = name;
}

Object::~Object()
{

}

void Object::SetBBox(const cv::Rect& rect)
{
	mBBox = rect;
}

void Object::SetBBox(int x, int y, int width, int height)
{
	mBBox = cv::Rect(x, y, width, height);
}

void Object::SetColor(uint8_t R, uint8_t G, uint8_t B) 
{
	mColor = cv::Scalar(B, G, R, 255);
}

void Object::SetConfidence(float conf)
{
	mConfidence = conf;
}

void Object::SetTitle(const std::string& title)
{
	mTitle = title;
}

void Object::Scale(double scale_x, double scale_y)
{
	mBBox.x *= scale_x;
	mBBox.y *= scale_y;
	mBBox.width *= scale_x;
	mBBox.height *= scale_y;
}
void Object::Offset(double offset_x, double offset_y)
{
	mBBox.x += offset_x;
	mBBox.y += offset_y;
}

void Object::DrawTo(cv::Mat img)
{
	cv::rectangle(img, mBBox, mColor, 2);
	double fontScale = 0.6;
	if (Props.ShowConfidence)
	{
		int conf = (int)(mConfidence * 100);
		std::string text = std::to_string(conf) + "%";
		cv::Point2d pos = cv::Point(mBBox.tl().x + 20, mBBox.y);
		cv::putText(img, text, pos, cv::FONT_HERSHEY_SIMPLEX, fontScale, mColor);
	}
	if (Props.ShowName && mName != "")
	{
		cv::Point2d pos = cv::Point(mBBox.x, mBBox.y + 20);
		cv::putText(img, mName, pos, cv::FONT_HERSHEY_SIMPLEX, fontScale, mColor);
	}
	if (Props.ShowTitle && mTitle != "")
	{
		cv::Point2d pos = cv::Point(mBBox.x, mBBox.y - 5);
		cv::putText(img, mTitle, pos, cv::FONT_HERSHEY_SIMPLEX, fontScale, mColor);
	}
}

cv::Point2f Object::GetCenter() const
{
	float cx = mBBox.x + mBBox.width / 2.0;
	float cy = mBBox.y + mBBox.height / 2.0;

	return cv::Point2f(cx, cy);
}