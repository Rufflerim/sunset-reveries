//
// Created by gaetz on 08/04/2023.
//

#include "ImRenderer.h"

#define RAYGUI_IMPLEMENTATION
#include "_Libs/raygui/raygui.h"

namespace render::im {
    f32 GuiSlider(const Rectangle& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue) {
        return ::GuiSlider(rect, textLeft.c_str(), textRight.c_str(), value, minValue, maxValue);
    }

    f32 GuiSliderBar(const Rectangle& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue) {
        return ::GuiSliderBar(rect, textLeft.c_str(), textRight.c_str(), value, minValue, maxValue);
    }

    f32 GuiProgressBar(const Rectangle& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue) {
        return ::GuiProgressBar(rect, textLeft.c_str(), textRight.c_str(), value, minValue, maxValue);
    }
}