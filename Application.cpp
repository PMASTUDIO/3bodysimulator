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
#include "Utils/Timestep.h"

namespace Simulator {
    static Application* s_Instance = nullptr;

    Application::Application(const ApplicationSpecification &spec)
    : m_Specification(spec) {
        m_LastX = m_Specification.width / 2;
        m_LastY = m_Specification.height / 2;

        bodies.emplace_back( 7.34e2, 1.740, glm::vec3{15.57f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 192.0f} );
        bodies.emplace_back(5.972e4, 6.371, glm::vec3{0.0f}, glm::vec3{0.0f, 0.0f, -192.0f} );
        bodies.emplace_back(5.972e4, 6.371, glm::vec3{0.0f, 25.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f} );

        m_Camera.MovementSpeed = 30.0f;

        s_Instance = this;
    }

    Application::~Application() {
        s_Instance = nullptr;
    }

    void Application::MouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (m_FirstMouse)
        {
            m_LastX = xpos;
            m_LastY = ypos;
            m_FirstMouse = false;
        }

        float xoffset = xpos - m_LastX;
        float yoffset = m_LastY - ypos; // reversed since y-coordinates go from bottom to top

        m_LastX = xpos;
        m_LastY = ypos;

        m_Camera.ProcessMouseMovement(xoffset, yoffset);
    }

    void Application::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
        m_Camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }

    void Application::FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
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

        glfwSetWindowUserPointer(m_Window, s_Instance);

        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            static_cast<Application*>(glfwGetWindowUserPointer(window))->FramebufferSizeCallback(window, width, height);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
            static_cast<Application*>(glfwGetWindowUserPointer(window))->MouseCallback(window, xPos, yPos);
        });
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            static_cast<Application*>(glfwGetWindowUserPointer(window))->ScrollCallback(window, xOffset, yOffset);
        });

        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // TRIANGLE TEST
        for (auto &body: bodies) {
            body.Init();
        }

        int initial_framebuffer_width, initial_framebuffer_height;
        glfwGetFramebufferSize(m_Window, &initial_framebuffer_width,
                                 &initial_framebuffer_height);

        glViewport(0, 0, initial_framebuffer_width, initial_framebuffer_height);
        glEnable(GL_DEPTH_TEST);


        while (!glfwWindowShouldClose(m_Window)) {
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            OnUpdate(timestep);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            OnRender();

            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::OnUpdate(Timestep ts) {
        for (auto &body: bodies) {
            body.UpdateVelocity(bodies, 0.01f);
        }

        for (auto &body: bodies) {
            body.UpdatePosition(0.01f);
        }

        m_Camera.OnUpdate(ts, m_Window);
    }

    void Application::OnRender() {
        glClearColor(0.0, 0.0, 0.0, 1.0);

        for (auto &body: bodies) {
            body.Render(m_Camera.GetViewMatrix());
        }
    }

    Application & Application::Get() {
        return *s_Instance;
    }
} // Simulator