//
// Created by gaetz on 07/11/2022.
//

#ifndef SUNSET_REVERIES_ENTITY_H
#define SUNSET_REVERIES_ENTITY_H

#include <array>
#include "Defines.h"
#include "Log.h"

constexpr i32 MAX_COMPONENTS = 3;

struct Entity {
    explicit Entity(u32 idP): id {idP} {}

    u32 id;
    std::array<i32, MAX_COMPONENTS> components { -1, -1, -1 };

    void LogEntity() {
        LOG(LogLevel::Debug) << components.at(0) << " " << components.at(1) << " " << components.at(2);
    }
};

#endif //SUNSET_REVERIES_ENTITY_H
