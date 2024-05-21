//
// Created by Pedro Ganme on 21/05/24.
//

#pragma once
#include <utility>

#include "../../Renderer/Input.h"

class DesktopInput : public Input {
protected:
    virtual bool IsKeyPressedImpl(int keycode) override;

    virtual bool IsMouseButtonPressedImpl(int button) override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
};
