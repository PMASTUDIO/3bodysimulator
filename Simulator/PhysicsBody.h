//
// Created by pganm on 5/17/2024.
//

#pragma once
#include <glm/glm.hpp>

#include "../Renderer/PrimitiveRenderer.h"

class PhysicsBody {
public:
    PhysicsBody(float mass, float radius, glm::vec3 velocity, glm::vec3 position);

    void Init();
    void Render(const glm::mat4 &projectionMat);

    glm::vec3 GetPosition() const { return m_Position; }
    glm::vec3 GetVelocity() const { return m_Velocity; }
    float GetMass() const { return m_Mass; }

    void UpdateVelocity(std::vector<PhysicsBody>& bodies, float dt);
    void UpdatePosition(float dt);
private:
    PrimitiveRenderer m_Primitive;

    float m_Mass;
    float m_Radius;
    glm::vec3 m_Velocity;
    glm::vec3 m_Position;
};


