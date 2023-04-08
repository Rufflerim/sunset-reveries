//
// Created by gaetz on 07/03/23.
//

#ifndef SUNSET_REVERIES_WORLD_STATE_HPP
#define SUNSET_REVERIES_WORLD_STATE_HPP

#include "Components.hpp"
#include "Entity.hpp"

#include <vector>
using std::vector;

struct WorldState {
    u32 currentFrame { 0 };
    vector<u64> entityIds;
    vector<Entity> entities;
    vector<Transform2D> transforms;
    vector<Sprite> sprites;
    vector<Rigidbody2D> bodies;
    vector<Replay> replays;

    template<class T>
    T& GetWorldStateComponent(u64 entityId) {
        if constexpr (std::is_same_v<T, Transform2D>) {
            return transforms.at(FindWorldStateEntityComponent(entityId, ComponentIndex::Transform2D));
        } else if constexpr (std::is_same_v<T, Sprite>) {
            return sprites.at(FindWorldStateEntityComponent(entityId, ComponentIndex::Sprite));
        } else if constexpr (std::is_same_v<T, Rigidbody2D>) {
            return bodies.at(FindWorldStateEntityComponent(entityId, ComponentIndex::Rigidbody2D));
        } else if constexpr (std::is_same_v<T, Replay>) {
            return replays.at(FindWorldStateEntityComponent(entityId, ComponentIndex::Replay));
        }
    }

private:
    Entity& FindWorldStateEntity(u64 entityId) {
        auto itr = std::lower_bound(entityIds.begin(), entityIds.end(), entityId);
        return entities.at(std::distance(entityIds.begin(), itr));
    }

    i32 FindWorldStateEntityComponent(u64 entityId, ComponentIndex componentIndex) {
        return FindWorldStateEntity(entityId).components.at(static_cast<i32>(componentIndex));
    }
};

#endif //SUNSET_REVERIES_WORLD_STATE_HPP
