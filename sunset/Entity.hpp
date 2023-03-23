//
// Created by gaetz on 07/11/2022.
//

#ifndef SUNSET_REVERIES_ENTITY_HPP
#define SUNSET_REVERIES_ENTITY_HPP

#include <array>
#include "Defines.hpp"
#include "Log.hpp"

constexpr i32 MAX_COMPONENTS = 4;

struct Entity {
    explicit Entity(u32 idP): id {idP} {}

    u32 id;
    std::array<i32, MAX_COMPONENTS> components { -1, -1, -1, -1 };

    void LogEntity() {
        LOG(LogLevel::Debug) << "Entity Id: " << id;
        LOG(LogLevel::Debug) << "Components: "<< components.at(0) << " " << components.at(1) << " " << components.at(2) << " " << components.at(3);
    }
};

#endif //SUNSET_REVERIES_ENTITY_HPP
