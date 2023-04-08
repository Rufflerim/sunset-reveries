//
// Created by gaetz on 08/04/2023.
//

#ifndef SUNSET_REVERIES_IM_RENDERER_H
#define SUNSET_REVERIES_IM_RENDERER_H

#include "raylib.h"
#include "Defines.hpp"

namespace render::im {
    GAPI f32 GuiSlider(const Rectangle& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue);
    GAPI f32 GuiSliderBar(const Rectangle& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue);
    GAPI f32 GuiProgressBar(const Rectangle& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue);
}

#endif //SUNSET_REVERIES_IM_RENDERER_H
