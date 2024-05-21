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

#include "Platform/Desktop/DesktopWindow.h"
#include "Renderer/Input.h"
#include "Renderer/PrimitiveRenderer.h"
#include "Utils/Timestep.h"

namespace Simulator {
    static Application* s_Instance = nullptr;

    Application::Application(const std::string& title, unsigned int width, unsigned int height){
        m_LastX = width / 2.0f;
        m_LastY = height / 2.0f;

        m_Window = std::unique_ptr<Window>(DesktopWindow::Create({ title, width, height }));
        m_Window->SetFramebufferSizeCallback([this](int width, int height) { FramebufferSizeCallback(width, height); });
        m_Window->SetScrollCallback([this](double xOffset, double yOffset) { ScrollCallback(xOffset, yOffset); });
        m_Window->SetCursorPosCallback([this](float xPos, int yPos) { MouseCallback(xPos, yPos); });
        m_Window->SetWindowCloseCallback([this]() { m_Running = false; });

        bodies.emplace_back( 7.34e2, 1.740, glm::vec3{15.57f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 192.0f} );
        bodies.emplace_back(5.972e4, 6.371, glm::vec3{0.0f}, glm::vec3{0.0f, 0.0f, -192.0f} );
        bodies.emplace_back(5.972e4, 6.371, glm::vec3{0.0f, 25.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f} );

        m_Camera.MovementSpeed = 60.0f;

        s_Instance = this;
    }

    Application::~Application() {
        s_Instance = nullptr;
    }

    void Application::MouseCallback(double xposIn, double yposIn) {
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

    void Application::ScrollCallback(double xoffset, double yoffset) {
        m_Camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }

    void Application::FramebufferSizeCallback(int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Application::Run() {
        glfwSetInputMode((GLFWwindow*)m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // TRIANGLE TEST
        for (auto &body: bodies) {
            body.Init();
        }

        int initial_framebuffer_width, initial_framebuffer_height;
        glfwGetFramebufferSize((GLFWwindow*)m_Window->GetNativeWindow(), &initial_framebuffer_width,
                                 &initial_framebuffer_height);

        glViewport(0, 0, initial_framebuffer_width, initial_framebuffer_height);
        glEnable(GL_DEPTH_TEST);

        while (m_Running) {
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            OnUpdate(timestep);


            OnRender();

            m_Window->OnUpdate();
        }

    }

    void Application::OnUpdate(Timestep ts) {
        if(Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
            m_Running = false;
        }

        for (auto &body: bodies) {
            body.UpdateVelocity(bodies, 0.01f);
        }

        for (auto &body: bodies) {
            body.UpdatePosition(0.01f);
        }

        m_Camera.OnUpdate(ts);
    }

    void Application::OnRender() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);

        for (auto &body: bodies) {
            body.Render(m_Camera.GetViewMatrix());
        }
    }

    Application & Application::Get() {
        return *s_Instance;
    }
} // Simulator