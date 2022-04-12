#ifndef _ObJECT_H_
#define _ObJECT_H_

#include <opencv2\opencv.hpp>

class Object
{
public:
	struct Properties
	{
		bool ShowName;
		bool ShowTitle;
		bool ShowConfidence;
		Properties()
		{
			ShowName = false;
			ShowTitle = true;
			ShowConfidence = false;
		}
	};

private:
	cv::Rect mBBox;
	cv::Scalar mColor;
	std::string mName;
	std::string mTitle;
	float mConfidence;
public:
	Properties Props;

	Object();
	Object(const std::string& name);
	Object(const std::string& title, const std::string& name);
	~Object();

	void SetBBox(const cv::Rect& rect);
	void SetBBox(int x, int y, int width, int height);
	void SetColor(uint8_t R, uint8_t G, uint8_t B);
	void SetConfidence(float conf);
	void SetTitle(const std::string& title);
	void Scale(double scale_x, double scale_y);
	void Offset(double offset_x, double offset_y);

	void DrawTo(cv::Mat img);

	cv::Point2f GetCenter() const;
	
};


#endif //_ObJECT_H_