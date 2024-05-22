//
// Created by pganm on 5/21/2024.
//

#pragma once
#include <vector>

#include "PhysicsBody.h"
#include "../Core/Layer.h"

class SimulationLayer : public Layer {
public:
    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(Timestep ts) override;

    void OnImGuiRender() override;
private:
    std::vector<PhysicsBody> bodies;
};
