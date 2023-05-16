//
// Created by gaetz on 07/11/2022.
//

#ifndef SUNSET_REVERIES_ENTITY_HPP
#define SUNSET_REVERIES_ENTITY_HPP

#include "Defines.hpp"
#include "Log.hpp"

constexpr i32 MAX_COMPONENTS = 6;

struct Entity {
    explicit Entity(u64 idP): id {idP} {}

    u64 id;
    array<i32, MAX_COMPONENTS> components { -1, -1, -1, -1, -1, -1 };

#ifdef GDEBUG
    void LogEntity() {
        LOG(LogLevel::Debug) << "Entity Id: " << id;
        LOG(LogLevel::Debug) << "Components: "<< components.at(0) << " " << components.at(1) << " "
        << components.at(2) << " " << components.at(3) << " " << components.at(4) << " " << components.at(5);
    }
#endif
};

#endif //SUNSET_REVERIES_ENTITY_HPP
