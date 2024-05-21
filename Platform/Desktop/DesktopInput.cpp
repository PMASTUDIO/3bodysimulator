//
// Created by Pedro Ganme on 21/05/24.
//

#include "DesktopInput.h"

#include <GLFW/glfw3.h>

#include "../../Application.h"

Input* Input::s_Instance = new DesktopInput();

bool DesktopInput::IsKeyPressedImpl(int keycode) {
    auto window = static_cast<GLFWwindow*>(Simulator::Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool DesktopInput::IsMouseButtonPressedImpl(int button) {
    auto window = static_cast<GLFWwindow*>(Simulator::Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> DesktopInput::GetMousePositionImpl() {
    auto window = static_cast<GLFWwindow*>(Simulator::Application::Get().GetWindow().GetNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}

float DesktopInput::GetMouseXImpl() {
    auto[x, y] = GetMousePositionImpl();
    return x;
}

float DesktopInput::GetMouseYImpl() {
    auto[x, y] = GetMousePositionImpl();
    return y;
}
