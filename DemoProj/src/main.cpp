#include <string>
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include <opencv2\video.hpp>
#include <json.hpp>

#include "Application.h"
#include "Detector.h"

bool loadAppConfig(const std::string& cfgFilename, AppConfig* pConfig)
{
	if (pConfig == NULL)
	{
		return false;
	}
	std::ifstream cfg_file;
	cfg_file.open(cfgFilename);
	if (cfg_file.is_open())
	{
		try {
			nlohmann::json j;
			cfg_file >> j;
			cfg_file.close();

			pConfig->ConfidenceThreshold = j["detection-conf-threshold"];
			pConfig->InputSource = j["input-source"];
			auto arr = j["restricted-area"];

			if (arr.size() % 2 == 0)
			{
				for (int i = 0; i < arr.size(); i += 2)
				{
					int x = (int)arr[i];
					int y = (int)arr[i + 1];
					pConfig->RestrictedArea.push_back(cv::Point(x, y));
				}
			}
		}
		catch (...)
		{
			return false;
		}
	}
	return true;
}

void main()
{
	AppConfig app_config;
	loadAppConfig(APP_CONFIG_FILE, &app_config);

	Application app(&app_config);
	app.Start();
}