//
// Created by pganm on 5/21/2024.
//

#include "SimulationLayer.h"

#include "../Application.h"

void SimulationLayer::OnAttach() {
    bodies.emplace_back( 7.34e2, 1.740, glm::vec3{15.57f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 192.0f} );
    bodies.emplace_back(5.972e4, 6.371, glm::vec3{0.0f}, glm::vec3{0.0f, 0.0f, -192.0f} );
    bodies.emplace_back(5.972e4, 6.371, glm::vec3{0.0f, 25.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f} );

    for (auto &body: bodies) {
        body.Init();
    }
}

void SimulationLayer::OnDetach() {

}

void SimulationLayer::OnUpdate(Timestep ts) {

    for (auto &body: bodies) {
        body.UpdateVelocity(bodies, 0.01f);
    }

    for (auto &body: bodies) {
        body.UpdatePosition(0.01f);
    }

    for (auto &body: bodies) {
        body.Render(Simulator::Application::Get().GetCamera().GetViewMatrix());
    }

}

void SimulationLayer::OnImGuiRender() {

}
