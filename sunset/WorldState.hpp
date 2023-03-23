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
    vector<u32> entityIds;
    vector<Entity> entities;
    vector<Transform2D> transforms;
    vector<Sprite> sprites;
    vector<Rigidbody2D> bodies;
    vector<RigidbodyRaycast2D> bodyRaycasts;
};

#endif //SUNSET_REVERIES_WORLD_STATE_HPP
