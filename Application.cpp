//
// Created by Pedro Ganme on 15/05/24.
//

#include "Application.h"

#include <iostream>
#include <GLFW/glfw3.h>

#ifdef WITH_EMSCRIPTEN
#else
    #include <glad/glad.h>
#endif

#include <random>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/PrimitiveRenderer.h"
#include "Renderer/Shader.h"



namespace Simulator {
    static Application* s_Instance = nullptr;

    PrimitiveRenderer sphereRenderer(PrimitiveType::SPHERE);

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
        sphereRenderer.Init();

        while (!glfwWindowShouldClose(m_Window)) {
            OnUpdate();

            glViewport(0, 0, m_Specification.width, m_Specification.height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            OnRender();

            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::OnUpdate() {
    }

    void Application::OnRender() {
        glClearColor(0.2, 0.2, 0.2, 1.0);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)m_Specification.width/(float)m_Specification.height, 0.1f, 100.0f);

        sphereRenderer.Render(proj);
    }

    Application & Application::Get() {
        return *s_Instance;
    }
} // Simulator