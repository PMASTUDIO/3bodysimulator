//
// Created by Pedro Ganme on 15/05/24.
//

#pragma once
#include <string>

#include <GLFW/glfw3.h>

#include "Renderer/Camera.h"

namespace Simulator {

    struct ApplicationSpecification
    {
        int width = 1920;
        int height = width / 16 * 9;
        std::string title = "3body Universe Simulator";
    };

    class Application {
    public:
        explicit Application(const ApplicationSpecification& spec = ApplicationSpecification());
        ~Application();

        GLFWwindow* GetRawWindow() const { return m_Window; };

        void Run();
        void OnUpdate(Timestep ts);
        void OnRender();
    public:
        static Application& Get();
    private:
        void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
        void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    private:
        ApplicationSpecification m_Specification;

        bool m_FirstMouse = true;
        float m_LastX, m_LastY;
        float m_LastFrameTime = 0.0f;

        Camera m_Camera{ glm::vec3(0.0f, 0.0f, 3.0f) };

        GLFWwindow* m_Window;
    };

} // Simulator
