#ifndef _OV_BUILD_IN_MODELS_H_
#define _OV_BUILD_IN_MODELS_H_

#include<string>
#include<vector>

class ModelInfo
{
private:
	unsigned char* pNetwork;
	unsigned char* pWeights;
	int mNetworkSize;
	int mWeightsSize;

    void release()
    {
        if (pNetwork != NULL)
        {
            memset(pNetwork, 0, mNetworkSize);
            delete[] pNetwork;
            pNetwork = NULL;
            mNetworkSize = 0;
        }
        if (pWeights != NULL)
        {
            memset(pWeights, 0, mWeightsSize);
            delete[] pWeights;
            pWeights = NULL;
            mWeightsSize = 0;
        }
    }

public:
	ModelInfo() : mNetworkSize(0), mWeightsSize(0), pNetwork(NULL), pWeights(NULL) {}

	~ModelInfo()
	{
        release();
	}

	void Create(int networkSize, int weightSize)
	{
        release();
		mNetworkSize = networkSize;
		mWeightsSize = weightSize;
		pNetwork = new unsigned char[mNetworkSize];
		pWeights = new unsigned char[mWeightsSize];
	}

	unsigned char* GetNetworkPtr() const 
	{
		return pNetwork;
	}

	unsigned char* GetWeightsPtr() const
	{
		return pWeights;
	}

	int GetNetworkSize() const
	{
		return mNetworkSize;
	}

	int GetWeightsSize() const
	{
		return mWeightsSize;
	}
};

class OVBuildInModels 
{
public:
    static std::string GetCacheName(const std::string& modelName);
	static bool GetModel(const std::string& name, ModelInfo& info);
	static int GetSupportedModels(std::vector<std::string>& models);
};

#endif	//_OV_BUILD_IN_MODELS_H_