#include "OVEngine.h"
#include "OVBuildInModels.h"

OVEngine::OVEngine()
{

}

OVEngine::~OVEngine()
{

}

bool OVEngine::CreateEngine(const std::string& modelName)
{
	mDeviceType = "";
	std::vector<std::string> availableDevices = mCore.GetAvailableDevices();
	for (int i = 0; i < availableDevices.size(); i++)
	{
		const std::string device = availableDevices[i].substr(0, 3);
		if (device == "GPU")
		{
			mDeviceType = device;
		}
		if (mDeviceType.empty())
		{
			if (device == "CPU")
			{
				mDeviceType = device;
			}
		}
	}
	if (mDeviceType.empty())
	{
		std::cout << "Not available device support OV" << std::endl;
		return false;
	}

	mModelName = modelName;

	ModelInfo info;
	if (OVBuildInModels::GetModel(mModelName, info))
	{
		std::string modelContent((char*)info.GetNetworkPtr(), info.GetNetworkSize());
		mNetwork = mCore.ReadNetwork(modelContent,
			make_shared_blob<uint8_t>({ Precision::U8,{ (size_t)info.GetWeightsSize() }, C }, (uint8_t*)info.GetWeightsPtr()));
	}
	else
	{
		std::cout << "Can not create model " << mModelName << std::endl;
		return false;
	}

	ICNNNetwork::InputShapes inputShapes = mNetwork.getInputShapes();
	mInputLayerName = inputShapes.begin()->first;
	SizeVector& inSizeVector = inputShapes.begin()->second;

	if (inSizeVector.size() != 4 || inSizeVector[1] != 3)
	{
		std::string err_msg("3-channel 4-dimensional model's input is expected");
		throw std::runtime_error(err_msg);
	}

	inSizeVector[0] = 1;  // set batch size to 1
	mNetwork.reshape(inputShapes);

	mInputChannels = inSizeVector[1];
	mInputHeight = inSizeVector[2];
	mInputWidth = inSizeVector[3];

	InputInfo& inputInfo = *mNetwork.getInputsInfo().begin()->second;
	inputInfo.setLayout(Layout::NHWC);
	inputInfo.setPrecision(Precision::U8);

	const OutputsDataMap& outputsDataMap = mNetwork.getOutputsInfo();
	if (outputsDataMap.size() != 1)
	{
		throw std::runtime_error("Output layer supports topologies only with 1 output");
	}

	mOutputLayerName = outputsDataMap.begin()->first;
	Data& data = *outputsDataMap.begin()->second;

	data.setPrecision(Precision::FP32);
	//data.setLayout(Layout::NHWC);
	data.setLayout(Layout::NCHW);

	const SizeVector& outSizeVector = data.getTensorDesc().getDims();
	switch (outSizeVector.size()) {
	case 3:
		mOutputChannels = 0;
		mOutputHeight = outSizeVector[1];
		mOutputWidth = outSizeVector[2];
		break;
	case 4:
		mOutputChannels = outSizeVector[1];
		mOutputHeight = outSizeVector[2];
		mOutputWidth = outSizeVector[3];
		break;
	default:
		throw std::runtime_error("Unexpected output blob shape. Only 4D and 3D output blobs are"
			"supported.");
	}

	

	if (mDeviceType == "GPU")
	{
		std::string model_cache_dir = "ov_cache";
		std::map<std::string, std::string> ov_config =
		{
			{ CONFIG_KEY(CACHE_DIR), model_cache_dir.c_str() },
		};
		mExeNetwork = mCore.LoadNetwork(mNetwork, mDeviceType, ov_config);
	}
	else 
	{
		mExeNetwork = mCore.LoadNetwork(mNetwork, mDeviceType);
	}

	mInferReq = mExeNetwork.CreateInferRequest();

	mOutputBlob = mInferReq.GetBlob(mOutputLayerName);

	return true;
}

Blob::Ptr OVEngine::GetBlob(const cv::Mat& mat)
{
	size_t channels = mat.channels();
	size_t width = mat.cols;
	size_t height = mat.rows;

	size_t strideH = mat.step.buf[0];
	size_t strideW = mat.step.buf[1];

	bool is_dense = ((strideW == channels)
		&& (strideH == channels * width));
	if (!is_dense) THROW_IE_EXCEPTION
		<< "Doesn't support conversion from not dense cv::Mat";

	TensorDesc tDesc(Precision::U8,
	{ 1, channels, height, width },
		Layout::NHWC);

	return make_shared_blob<uint8_t>(tDesc, mat.data);
}

/*
void OVEngine::SetBlob(const std::string& layerName, const Blob::Ptr& blob)
{
	mInferReq.SetBlob(layerName, blob);
}

Blob::Ptr OVEngine::GetBlob(const std::string& layerName)
{
	return mInferReq.GetBlob(layerName);
}

void OVEngine::Infer()
{
	mInferReq.Infer();
}
*/

const void* OVEngine::Infer(const cv::Mat& mat)
{
	Blob::Ptr blob = GetBlob(mat);
	mInferReq.SetBlob(mInputLayerName, blob);
	mInferReq.Infer();
	mOutputBlob = mInferReq.GetBlob(mOutputLayerName);
	return mOutputBlob->cbuffer().as<void*>();
}

const void* OVEngine::Infer(const Blob::Ptr& data)
{
	mInferReq.SetBlob(mInputLayerName, data);
	mInferReq.Infer();
	mOutputBlob = mInferReq.GetBlob(mOutputLayerName);
	return mOutputBlob->buffer();
}

void OVEngine::GetInputLayerInfo(int& channels, int& width, int& height)
{
	channels = mInputChannels;
	width = mInputWidth;
	height = mInputHeight;
}

void OVEngine::GetOuputLayerInfo(int& channels, int& width, int& height)
{
	channels = mOutputChannels;
	width = mOutputWidth;
	height = mOutputHeight;
}
