//
// Created by Pedro Ganme on 15/05/24.
//

#pragma once
#include <string>

#include <GLFW/glfw3.h>

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

        void Run();
        void OnUpdate();
        void OnRender();
    public:
        static Application& Get();
    private:
        ApplicationSpecification m_Specification;

        GLFWwindow* m_Window;
    };

} // Simulator
