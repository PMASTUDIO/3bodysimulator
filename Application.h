//
// Created by Pedro Ganme on 15/05/24.
//

#pragma once
#include <string>

#include <GLFW/glfw3.h>

#include "Renderer/Camera.h"
#include "Renderer/Window.h"
#include "Simulator/PhysicsBody.h"

namespace Simulator {

    class Application {
    public:
        explicit Application(const std::string& title = "3body Universe Simulator", unsigned int width = 1280, unsigned int height = 720);
        virtual ~Application();

        void Run();
        void OnUpdate(Timestep ts);
        void OnRender();
    public:
        static Application& Get();
    private:
        void MouseCallback(double xposIn, double yposIn);
        void ScrollCallback(double xoffset, double yoffset);
        void FramebufferSizeCallback(int width, int height);
    private:
        std::vector<PhysicsBody> bodies;

        bool m_FirstMouse = true;
        float m_LastX, m_LastY;
        float m_LastFrameTime = 0.0f;

        Camera m_Camera{ glm::vec3(0.0f, 0.0f, 0.0f) };

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

} // Simulator
