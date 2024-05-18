//
// Created by pganm on 5/17/2024.
//

#include "PhysicsBody.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <utility>

#include <iostream>

PhysicsBody::PhysicsBody(float mass, float radius, glm::vec3 velocity, glm::vec3 position) : m_Mass(mass), m_Radius(radius), m_Velocity(velocity), m_Position(position), m_Primitive(PrimitiveType::SPHERE, 1.0f) {
}

void PhysicsBody::Init() {
    m_Primitive.Init();
    m_Primitive.SetPosition(m_Position);
}

void PhysicsBody::Render(const glm::mat4 &projectionMat) {
    m_Primitive.Render(projectionMat);
}

void PhysicsBody::UpdateVelocity(std::vector<PhysicsBody>& bodies, float dt) {
    for (PhysicsBody &body: bodies) {
        if(&body != this) {
            const float distanceSqrd = glm::length2(body.GetPosition() - GetPosition());

            constexpr float G = 0.0001f; // N * m^2 * kg^-2
            const glm::vec3 forceDir =  glm::normalize(body.GetPosition() - GetPosition());
            const glm::vec3 force = forceDir * G * (GetMass() * body.GetMass() / distanceSqrd);
            const glm::vec3 acceleration = force / GetMass();

            m_Velocity += acceleration * dt;
        }
    }

    // std::cout << "Velocity X: " << m_Velocity.x << " Y: " << m_Velocity.y << " Z: " << m_Velocity.z << std::endl;
}

void PhysicsBody::UpdatePosition(float dt) {
    m_Position += m_Velocity * dt;
    // std::cout << "X: " << m_Position.x << " Y: " << m_Position.y << " Z: " << m_Position.z << std::endl;

    m_Primitive.SetPosition(m_Position);
}
