//
// Created by Pedro Ganme on 16/05/24.
//

#include "PrimitiveRenderer.h"

# define M_PI           3.14159265358979323846
#include <math.h>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

PrimitiveRenderer::PrimitiveRenderer(PrimitiveType type, float scale)  {
    switch (type) {
        case PrimitiveType::SPHERE: {
            m_Geometry = GenerateSphere(scale);
            break;
        }
        case PrimitiveType::CUBE: {
            m_Geometry = GenerateCube(scale);
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_Geometry.stride, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PrimitiveRenderer::Render(glm::mat4 projection) {
    glUseProgram(m_Shader);

    int projLoc = glGetUniformLocation(m_Shader, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	int modelLoc = glGetUniformLocation(m_Shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_ModelMatrix));

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_Geometry.indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // glBindVertexArray(0);
}

void PrimitiveRenderer::SetPosition(const glm::vec3& pos) {
	m_ModelMatrix = glm::translate(glm::mat4{1.0f}, pos);
}

PrimitiveGeometry PrimitiveRenderer::GenerateSphere(float radius) {
	int sectorCount = 36;
	int stackCount = 18;

    int i, j;
    std::vector<float> sphere_vertices;
    std::vector<int> sphere_indices;
    float x, y, z, xy;                              // vertex position
	float lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = (float)(2 * M_PI / sectorCount);
	float stackStep = (float)(M_PI / stackCount);
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = (float)(M_PI / 2 - i * stackStep);        // starting from pi/2 to -pi/2
		xy = 1.02f * radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);				// r * cos(u) * sin(v)
			sphere_vertices.push_back(x);
			sphere_vertices.push_back(y);
			sphere_vertices.push_back(z);


			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			//sphere_vertices.push_back(s);
			//sphere_vertices.push_back(t);

		}
	}
	/* GENERATE VERTEX ARRAY */


	/* GENERATE INDEX ARRAY */
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				sphere_indices.push_back(k1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				sphere_indices.push_back(k1 + 1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k2 + 1);
			}
		}
	}

    return PrimitiveGeometry{
        sphere_vertices,
        std::vector<float>{},
        sphere_indices,
        3 * sizeof(float)
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
        3 * sizeof(float)
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
        3 * sizeof(float)
    };
}
