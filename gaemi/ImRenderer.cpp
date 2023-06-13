//
// Created by gaetz on 08/04/2023.
//

#include "ImRenderer.h"

#define RAYGUI_IMPLEMENTATION
#include "_Libs/raygui/raygui.h"

#include "Rect.hpp"

using gmath::Rect;

namespace render::im {
    f32 GuiSlider(const Rect& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue) {
        return ::GuiSlider(rect.ToRaylib(), textLeft.c_str(), textRight.c_str(), value, minValue, maxValue);
    }

    f32 GuiSliderBar(const Rect& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue) {
        return ::GuiSliderBar(rect.ToRaylib(), textLeft.c_str(), textRight.c_str(), value, minValue, maxValue);
    }

    f32 GuiProgressBar(const Rect& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue) {
        return ::GuiProgressBar(rect.ToRaylib(), textLeft.c_str(), textRight.c_str(), value, minValue, maxValue);
    }
}