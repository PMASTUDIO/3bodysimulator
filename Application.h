//
// Created by Pedro Ganme on 15/05/24.
//

#pragma once
#include <string>
#include <memory>

#include <GLFW/glfw3.h>

#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Core/Timestep.h"
#include "Renderer/Camera.h"
#include "Core/Window.h"
#include "Simulator/PhysicsBody.h"
#include "Simulator/SimulationLayer.h"

namespace Simulator {

    class Application {
    public:
        explicit Application(const std::string& title = "3body Universe Simulator", unsigned int width = 1280, unsigned int height = 720);
        virtual ~Application();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() { return *m_Window; }
        inline Camera& GetCamera() { return m_Camera; }

        void Run();
        void OnUpdate(Timestep ts);
    public:
        static Application& Get();
    private:
        void MouseCallback(double xposIn, double yposIn);
        void ScrollCallback(double xoffset, double yoffset);
        void FramebufferSizeCallback(int width, int height);
    private:
        bool m_FirstMouse = true;
        float m_LastX, m_LastY;
        float m_LastFrameTime = 0.0f;

        Camera m_Camera{ glm::vec3(0.0f, 0.0f, 0.0f) };

        LayerStack m_LayerStack;
        SimulationLayer* m_SimulationLayer;

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

} // Simulator
