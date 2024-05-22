//
// Created by Pedro Ganme on 21/05/24.
//

#pragma once
#include <functional>
#include <string>

struct WindowProps
{
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(const std::string& title = "3body Universe Simulator",
                unsigned int width = 1280,
                unsigned int height = 720)
        : Title(title), Width(width), Height(height)
    {
    }
};

class Window {
public:
    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    // Window attributes
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void SetFramebufferSizeCallback(const std::function<void(int, int)>& callback) = 0;
    virtual void SetCursorPosCallback(const std::function<void(double, double)>& callback) = 0;
    virtual void SetScrollCallback(const std::function<void(double, double)>& callback) = 0;
    virtual void SetWindowCloseCallback(const std::function<void()>& callback) = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const WindowProps& props = WindowProps());
};
