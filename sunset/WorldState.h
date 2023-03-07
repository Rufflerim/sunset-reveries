//
// Created by gaetz on 07/03/23.
//

#ifndef SUNSET_REVERIES_WORLDSTATE_H
#define SUNSET_REVERIES_WORLDSTATE_H

#include <vector>

using std::vector;

struct WorldState {
    vector<Entity> entities;
    vector<Transform2D> transforms;
    vector<Sprite> sprites;
    vector<Rigidbody2D> bodies;
};

#endif //SUNSET_REVERIES_WORLDSTATE_H
