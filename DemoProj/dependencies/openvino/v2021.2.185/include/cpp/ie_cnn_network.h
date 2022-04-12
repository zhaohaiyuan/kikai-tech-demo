// Copyright (C) 2018-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

/**
 * @brief A header file that provides wrapper for ICNNNetwork object
 *
 * @file ie_cnn_network.h
 */
#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ie_icnn_network.hpp"
#include "ie_blob.h"
#include "ie_common.h"
#include "ie_data.h"
#include "details/ie_exception_conversion.hpp"
#include "ie_extension.h"

namespace ngraph {

class Function;

}  // namespace ngraph

namespace InferenceEngine {

/**
 * @brief This class contains all the information about the Neural Network and the related binary information
 */
class INFERENCE_ENGINE_API_CLASS(CNNNetwork) {
public:
    /**
     * @brief A default constructor
     */
    CNNNetwork() = default;

    /**
     * @brief Allows helper class to manage lifetime of network object
     *
     * @param network Pointer to the network object
     */
    explicit CNNNetwork(std::shared_ptr<ICNNNetwork> network): network(network) {
        actual = network.get();
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
    }

    /**
     * @brief A constructor from ngraph::Function object
     * This constructor wraps existing ngraph::Function
     * If you want to avoid modification of original Function, please create a copy
     * @param network Pointer to the ngraph::Function object
     * @param exts Vector of pointers to IE extension objects
     */
    explicit CNNNetwork(const std::shared_ptr<ngraph::Function>& network,
                        const std::vector<IExtensionPtr>& exts = {});

    /**
     * @brief A destructor
     */
    virtual ~CNNNetwork() {}

    /**
     * @copybrief ICNNNetwork::getOutputsInfo
     *
     * Wraps ICNNNetwork::getOutputsInfo
     *
     * @return outputs Reference to the OutputsDataMap object
     */
    virtual OutputsDataMap getOutputsInfo() const {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        OutputsDataMap outputs;
        actual->getOutputsInfo(outputs);
        return outputs;
    }

    /**
     * @copybrief ICNNNetwork::getInputsInfo
     *
     * Wraps ICNNNetwork::getInputsInfo
     *
     * @return inputs Reference to InputsDataMap object
     */
    virtual InputsDataMap getInputsInfo() const {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        InputsDataMap inputs;
        actual->getInputsInfo(inputs);
        return inputs;
    }

    /**
     * @copybrief ICNNNetwork::layerCount
     *
     * Wraps ICNNNetwork::layerCount
     *
     * @return The number of layers as an integer value
     */
    size_t layerCount() const {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        return actual->layerCount();
    }

    /**
     * @copybrief ICNNNetwork::getName
     *
     * Wraps ICNNNetwork::getName
     *
     * @return Network name
     */
    const std::string& getName() const {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        return actual->getName();
    }

    /**
     * @copybrief ICNNNetwork::setBatchSize
     *
     * Wraps ICNNNetwork::setBatchSize
     *
     * @param size Size of batch to set
     */
    virtual void setBatchSize(const size_t size) {
        CALL_STATUS_FNC(setBatchSize, size);
    }

    /**
     * @copybrief ICNNNetwork::getBatchSize
     *
     * Wraps ICNNNetwork::getBatchSize
     *
     * @return The size of batch as a size_t value
     */
    virtual size_t getBatchSize() const {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        return actual->getBatchSize();
    }

    /**
     * @brief An overloaded operator cast to get pointer on current network
     *
     * @return A shared pointer of the current network
     */
    operator ICNNNetwork::Ptr() {
        return network;
    }

    /**
     * @brief An overloaded operator & to get current network
     *
     * @return An instance of the current network
     */
    operator ICNNNetwork&() {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        return *actual;
    }

    /**
     * @brief An overloaded operator & to get current network
     *
     * @return A const reference of the current network
     */
    operator const ICNNNetwork&() const {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        return *actual;
    }

    /**
     * @brief Returns constant nGraph function
     *
     * @return constant nGraph function
     */
    std::shared_ptr<ngraph::Function> getFunction() {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        return actual->getFunction();
    }

    /**
     * @brief Returns constant nGraph function
     *
     * @return constant nGraph function
     */
    std::shared_ptr<const ngraph::Function> getFunction() const {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        return actual->getFunction();
    }

    /**
     * @copybrief ICNNNetwork::addOutput
     *
     * Wraps ICNNNetwork::addOutput
     *
     * @param layerName Name of the layer
     * @param outputIndex Index of the output
     */
    void addOutput(const std::string& layerName, size_t outputIndex = 0) {
        CALL_STATUS_FNC(addOutput, layerName, outputIndex);
    }

    /**
     * @brief Helper method to get collect all input shapes with names of corresponding Data objects
     *
     * @return Map of pairs: input name and its dimension.
     */
    virtual ICNNNetwork::InputShapes getInputShapes() const {
        if (actual == nullptr) THROW_IE_EXCEPTION << "CNNNetwork was not initialized.";
        ICNNNetwork::InputShapes shapes;
        InputsDataMap inputs;
        actual->getInputsInfo(inputs);
        for (const auto& pair : inputs) {
            auto info = pair.second;
            if (info) {
                auto data = info->getInputData();
                if (data) {
                    shapes[data->getName()] = data->getTensorDesc().getDims();
                }
            }
        }
        return shapes;
    }

    /**
     * @brief Run shape inference with new input shapes for the network
     *
     * @param inputShapes - map of pairs: name of corresponding data and its dimension.
     */
    virtual void reshape(const ICNNNetwork::InputShapes& inputShapes) {
        CALL_STATUS_FNC(reshape, inputShapes);
    }

    /**
     * @brief Serialize network to IR and weights files.
     *
     * @param xmlPath Path to output IR file.
     * @param binPath Path to output weights file. The parameter is skipped in case
     * of executable graph info serialization.
     */
    void serialize(const std::string& xmlPath, const std::string& binPath = "") const {
        CALL_STATUS_FNC(serialize, xmlPath, binPath);
    }

protected:
    /**
     * @brief Network extra interface, might be nullptr
     */
    std::shared_ptr<ICNNNetwork> network;

    /**
     * @brief A pointer to the current network
     */
    ICNNNetwork* actual = nullptr;
    /**
     * @brief A pointer to output data
     */
    DataPtr output;
};

}  // namespace InferenceEngine
