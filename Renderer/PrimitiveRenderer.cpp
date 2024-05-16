//
// Created by Pedro Ganme on 16/05/24.
//

#include "PrimitiveRenderer.h"

# define M_PI           3.14159265358979323846

#include <glad/glad.h>

#include "Shader.h"

PrimitiveRenderer::PrimitiveRenderer(PrimitiveType type)  {
    switch (type) {
        case PrimitiveType::SPHERE: {
            m_Geometry = GenerateSphere(20.0f);
            break;
        }
        case PrimitiveType::CUBE: {
            m_Geometry = GenerateCube(1.0f);
            break;
        }
        case PrimitiveType::TRIANGLE: {
            m_Geometry = GenerateTriangle();
            break;
        }
    }
}

void PrimitiveRenderer::Init() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Geometry.vertices.size(), m_Geometry.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * m_Geometry.indices.size(), m_Geometry.indices.data(), GL_STATIC_DRAW);

    Shader* shaders = Shader::LoadFromFile(std::vector<ShaderFile>{
        ShaderFile{"../Assets/Shaders/triangle_vertex.glsl", GL_VERTEX_SHADER},
        ShaderFile{"../Assets/Shaders/triangle_frag.glsl", GL_FRAGMENT_SHADER}
    });

    m_Shader = shaders->GetRendererID();
    glUseProgram(shaders->GetRendererID());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PrimitiveRenderer::Render() {
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glUseProgram(m_Shader);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_Geometry.indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // glBindVertexArray(0);
}

PrimitiveGeometry PrimitiveRenderer::GenerateSphere(float radius) {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<int> indices;
    std::vector<int> lineIndices;
    std::vector<float> interleavedVertices;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    const int sectorCount = 72;
    const int stackCount = 24;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            // s = (float)j / sectorCount;
            // t = (float)i / stackCount;
            // texCoords.push_back(s);
            // texCoords.push_back(t);
        }
    }


    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    std::size_t i, j;
    std::size_t count = vertices.size();
    for(i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i+1]);
        interleavedVertices.push_back(vertices[i+2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i+1]);
        interleavedVertices.push_back(normals[i+2]);

        // interleavedVertices.push_back(texCoords[j]);
        // interleavedVertices.push_back(texCoords[j+1]);
    }

    return PrimitiveGeometry{
        interleavedVertices,
        normals,
        indices,
        lineIndices,
        32
    };
}

PrimitiveGeometry PrimitiveRenderer::GenerateCube(float length) {
    std::vector<int> indices {
        //Top
        2, 6, 7,
        2, 3, 7,

        //Bottom
        0, 4, 5,
        0, 1, 5,

        //Left
        0, 2, 6,
        0, 4, 6,

        //Right
        1, 3, 7,
        1, 5, 7,

        //Front
        0, 2, 3,
        0, 1, 3,

        //Back
        4, 6, 7,
        4, 5, 7
    };


    std::vector<float> vertices {
        -1 * length, -1 * length,  0.5f * length, //0
         1 * length, -1 * length,  0.5f * length, //1
        -1 * length,  1 * length,  0.5f * length, //2
         1 * length,  1 * length,  0.5f * length, //3
        -1* length, -1* length, -0.5f* length, //4
         1* length, -1* length, -0.5f* length, //5
        -1* length,  1* length, -0.5f* length, //6
         1* length,  1* length, -0.5f* length  //7
    };

    return PrimitiveGeometry{
        vertices,
        std::vector<float>{},
        indices,
        std::vector<int>{},
        3
    };
}

PrimitiveGeometry PrimitiveRenderer::GenerateTriangle() {
    std::vector<int> indices {
        0, 1, 2
    };

    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f, 0.5f, 0.0f
    };

    return PrimitiveGeometry{
        vertices,
        std::vector<float>{},
        indices,
        std::vector<int>{},
        3 * sizeof(float)
    };
}
