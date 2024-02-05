//
// Created by gaetz on 16/09/2023.
//

#ifndef GCAUSALITY_WORLDSTATEMANAGER_HPP
#define GCAUSALITY_WORLDSTATEMANAGER_HPP

#include "Defines.hpp"
#include "WorldState.hpp"

namespace gcausality {

class WorldStateManager {
public:
    void Store(WorldState worldStateCopy);
    u64 GetCurrentFrame() const  { return currentFrame; }
private:
    u64 currentTimeMs { 0 };
    u64 currentFrame { 0 };
    vector<WorldState> states;
};

}

#endif //GCAUSALITY_WORLDSTATEMANAGER_HPP
