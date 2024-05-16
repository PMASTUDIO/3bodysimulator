//
// Created by Pedro Ganme on 15/05/24.
//

#include "Application.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <random>
#include <vector>
#include <string>

#include "Renderer/Shader.h"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f, 0.5f, 0.0f
};

namespace Simulator {
    static Application* s_Instance = nullptr;

    Application::Application(const ApplicationSpecification &spec)
    : m_Specification(spec)
    {
        s_Instance = this;
    }

    Application::~Application() {
        s_Instance = nullptr;
    }

    void Application::Run() {
        if (!glfwInit())
            exit(EXIT_FAILURE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_Window = glfwCreateWindow(m_Specification.width, m_Specification.height, m_Specification.title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(m_Window);
        gladLoadGL();
        glfwSwapInterval(1);

        // TRIANGLE TEST
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        Shader* shaders = Shader::LoadFromFile(std::vector<ShaderFile>{
            ShaderFile{"../Assets/Shaders/triangle_vertex.glsl", GL_VERTEX_SHADER},
            ShaderFile{"../Assets/Shaders/triangle_frag.glsl", GL_FRAGMENT_SHADER}
        });

        glUseProgram(shaders->GetRendererID());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        while (!glfwWindowShouldClose(m_Window)) {
            OnUpdate();

            glViewport(0, 0, m_Specification.width, m_Specification.height);
            glClear(GL_COLOR_BUFFER_BIT);
            OnRender();

            glUseProgram(shaders->GetRendererID());
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::OnUpdate() {
    }

    void Application::OnRender() {


        glClearColor(0.75, 0.22, 0.36, 1.0);
    }

    Application & Application::Get() {
        return *s_Instance;
    }
} // Simulator