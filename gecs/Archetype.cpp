//
// Created by gaetz on 02/07/2023.
//

#include "Archetype.hpp"
namespace gecs {

    size_t Archetype::GetRowCount() const {
        if (components.empty()) return 0;
        return components[0].Count();
    }
}
