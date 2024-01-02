//
// Created by gaetz on 26/07/2023.
//

#include "Types.hpp"

#include <sstream>
#include "GMath.hpp"

namespace gecs {
    str ComponentIdToString(ComponentId componentId) {
        switch (componentId) {
            case ComponentId::Position: {
                return "Pos";
            }
            case ComponentId::Velocity: {
                return "Vel";
            }
            case ComponentId::Sprite: {
                return "Spr";
            }
            default: {
                return "ABSURD";
            }
        }
    }

    ComponentId IndexToComponentId(i32 index) {
        switch (index) {
            case 0: return ComponentId::Position;
            case 1: return ComponentId::Velocity;
            case 2: return ComponentId::Sprite;
            default: return ComponentId::Position;
        }
    }

    str ArchetypeIdToString(ArchetypeId id) {
        if (id.none()) return "Empty";
        std::stringstream stream;
        for (i32 i = 0; i < MAX_COMPONENTS; ++i) {
            if (id.test(i)) {
                stream << ComponentIdToString(IndexToComponentId(i)) << "/";
            }
        }
        // Remove last character
        str components = stream.str();
        return components.substr(0, components.size() - 1);
    }

    u32 ComponentIdToBitValue(ComponentId componentId) {
        f32 id = static_cast<f32>(componentId);
        return gmath::Floor(gmath::Pow(2, id));
    }
}