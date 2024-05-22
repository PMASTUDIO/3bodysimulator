//
// Created by Pedro Ganme on 21/05/24.
//

#pragma once

#include <GLFW/glfw3.h>
#include "../../Core/Window.h"

class DesktopWindow : public Window {
public:
    DesktopWindow(const WindowProps& props);

    virtual ~DesktopWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override { return m_Data.Width; }
    inline unsigned int GetHeight() const override { return m_Data.Height; }

    // Window attributes
    inline void SetFramebufferSizeCallback(const std::function<void(int, int)>& callback) override { m_Data.FramebufferSizeCallback = callback; }
    inline void SetCursorPosCallback(const std::function<void(double, double)>& callback) override { m_Data.CursorPosCallback = callback; }
    inline void SetScrollCallback(const std::function<void(double, double)>& callback) override { m_Data.ScrollCallback = callback; }
    inline void SetWindowCloseCallback(const std::function<void()>& callback) override { m_Data.WindowCloseCallback = callback; };

    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    inline virtual void* GetNativeWindow() const { return m_Window; }
private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();
private:
    GLFWwindow* m_Window;

    struct WindowData
    {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        std::function<void(int, int)> FramebufferSizeCallback;
        std::function<void(double, double)> CursorPosCallback;
        std::function<void(double, double)> ScrollCallback;
        std::function<void()> WindowCloseCallback;
    };

    WindowData m_Data;
};

