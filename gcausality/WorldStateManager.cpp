//
// Created by gaetz on 16/09/2023.
//

#include "WorldStateManager.hpp"

namespace gcausality {

    void WorldStateManager::Store(gcausality::WorldState worldStateCopy) {
        currentFrame = states.size();
        worldStateCopy.currentFrame = currentFrame;
        //ecs->SetWorldState(newState);
        states.push_back(std::move(worldStateCopy));
    }
}
