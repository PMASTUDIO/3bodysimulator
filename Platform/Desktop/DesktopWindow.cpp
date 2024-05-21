//
// Created by Pedro Ganme on 21/05/24.
//

#include "DesktopWindow.h"

#include <iostream>
#include <glad/glad.h>

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << ")" << std::endl;
}

Window* Window::Create(const WindowProps& props) {
    return new DesktopWindow(props);
}

DesktopWindow::DesktopWindow(const WindowProps &props) {
    Init(props);
}

DesktopWindow::~DesktopWindow() {
    Shutdown();
}

void DesktopWindow::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void DesktopWindow::SetVSync(bool enabled) {
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_Data.VSync = enabled;
}

bool DesktopWindow::IsVSync() const {
    return m_Data.VSync;
}

void DesktopWindow::Init(const WindowProps &props) {
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    if (!s_GLFWInitialized)
    {
        int success = glfwInit();
        if(!success) exit(EXIT_FAILURE);
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(m_Window);
    gladLoadGL();
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;
        data.FramebufferSizeCallback(width, height);
    });
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.CursorPosCallback(xPos, yPos);
    });
    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.ScrollCallback(xOffset, yOffset);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.WindowCloseCallback();
    });
}

void DesktopWindow::Shutdown() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}
