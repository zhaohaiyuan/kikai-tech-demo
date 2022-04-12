#include "OVBuildInModels.h"
#include "pedestrian_detection_adas_0002.h"

bool OVBuildInModels::GetModel(const std::string& name, ModelInfo& info) {
	const unsigned char* p_network = NULL;
	const unsigned char* p_weights = NULL;
	int network_sz = 0;
	int weights_sz = 0;

	if ("pedestrian_detection_adas_0002" == name) {
		get_pedestrian_detection_adas_0002_info(network_sz, weights_sz);
		info.Create(network_sz, weights_sz);
		get_pedestrian_detection_adas_0002_data(info.GetNetworkPtr(), info.GetWeightsPtr());
		return true;
	}
	return false;
}
std::string OVBuildInModels::GetCacheName(const std::string& modelName) {
	if("pedestrian_detection_adas_0002" == modelName) return "bdaba494ffa51ca35cfd26134d9ae6de";
	return "";
}
int OVBuildInModels::GetSupportedModels(std::vector<std::string>& models) {
	models.clear();
	models.push_back("pedestrian_detection_adas_0002");
	return models.size();
}
