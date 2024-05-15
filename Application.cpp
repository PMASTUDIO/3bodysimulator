//
// Created by Pedro Ganme on 15/05/24.
//

#include "Application.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <random>

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

        m_Window = glfwCreateWindow(m_Specification.width, m_Specification.height, m_Specification.title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(m_Window);
        gladLoadGL();
        glfwSwapInterval(1);

        while (!glfwWindowShouldClose(m_Window)) {
            OnUpdate();

            glViewport(0, 0, m_Specification.width, m_Specification.height);
            glClear(GL_COLOR_BUFFER_BIT);
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
        glClearColor(0.75, 0.22, 0.36, 1.0);
    }

    Application & Application::Get() {
        return *s_Instance;
    }
} // Simulator