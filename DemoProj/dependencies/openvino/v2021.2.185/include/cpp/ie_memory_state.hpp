// Copyright (C) 2018-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

/**
 * @brief A header file that provides wrapper classes for IVariableState
 *
 * @file ie_memory_state.hpp
 */

#pragma once

#include <string>

#include "ie_imemory_state.hpp"
#include "details/ie_exception_conversion.hpp"
#include "details/ie_so_loader.h"

namespace InferenceEngine {

/**
 * @brief C++ exception based error reporting wrapper of API class IVariableState
 */
class VariableState {
    IVariableState::Ptr actual = nullptr;
    details::SharedObjectLoader::Ptr plugin = {};

public:
    /**
     * @brief constructs VariableState from the initialized shared_pointer
     * @param pState Initialized shared pointer
     * @param plg Optional: Plugin to use. This is required to ensure that VariableState can work properly even if plugin object is destroyed.
     */
    explicit VariableState(IVariableState::Ptr pState, details::SharedObjectLoader::Ptr plg = {}) : actual(pState), plugin(plg) {
        if (actual == nullptr) {
            THROW_IE_EXCEPTION << "VariableState wrapper was not initialized.";
        }
    }

    /**
     * @copybrief IVariableState::Reset
     *
     * Wraps IVariableState::Reset
     */
    void Reset() {
        CALL_STATUS_FNC_NO_ARGS(Reset);
    }

    /**
     * @copybrief IVariableState::GetName
     *
     * Wraps IVariableState::GetName
     * @return A string representing a state name
     */
    std::string GetName() const {
        char name[256];
        CALL_STATUS_FNC(GetName, name, sizeof(name));
        return name;
    }

    /**
     * @copybrief IVariableState::GetState
     *
     * Wraps IVariableState::GetState
     * @return A blob representing a state 
     */
    Blob::CPtr GetState() const {
        Blob::CPtr stateBlob;
        CALL_STATUS_FNC(GetState, stateBlob);
        return stateBlob;
    }

    /**
     * @copybrief IVariableState::GetLastState
     * @deprecated Use IVariableState::SetState instead
     *
     * Wraps IVariableState::GetLastState
     * @return A blob representing a last state 
     */
    INFERENCE_ENGINE_DEPRECATED("Use VariableState::GetState function instead")
    Blob::CPtr GetLastState() const {
        return GetState();
    }

    /**
     * @copybrief IVariableState::SetState
     *
     * Wraps IVariableState::SetState
     * @param state The current state to set
     */
    void SetState(Blob::Ptr state) {
        CALL_STATUS_FNC(SetState, state);
    }
};

/**
 * @brief For compatibility reasons.
 */
using MemoryState = VariableState;

}  // namespace InferenceEngine
