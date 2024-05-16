//
// Created by Pedro Ganme on 16/05/24.
//

#pragma once

#include <vector>

#include <glm/glm.hpp>

enum class PrimitiveType {
    SPHERE,
    CUBE,
    TRIANGLE
};

struct PrimitiveGeometry {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<int> indices;

    int stride;
};

class PrimitiveRenderer {
public:
    PrimitiveRenderer(PrimitiveType type);

    void Init();
    void Render(glm::mat4 projection);
private:
    static PrimitiveGeometry GenerateSphere(float radius);
    static PrimitiveGeometry GenerateCube(float length);
    static PrimitiveGeometry GenerateTriangle();
private:
    unsigned int m_VAO{}, m_VBO{}, m_IBO{};
    unsigned int m_Shader{};

    PrimitiveGeometry m_Geometry;
};

