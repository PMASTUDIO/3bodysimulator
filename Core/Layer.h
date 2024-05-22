//
// Created by pganm on 5/21/2024.
//

#pragma once

#include <string>

#include "Timestep.h"

class Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnImGuiRender() {}

    inline const std::string& GetName() const { return m_DebugName; }
protected:
    std::string m_DebugName;
};
