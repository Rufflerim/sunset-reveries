//
// Created by gaetz on 07/03/23.
//

#ifndef SUNSET_REVERIES_WORLDSTATEMANAGER_HPP
#define SUNSET_REVERIES_WORLDSTATEMANAGER_HPP

#include "WorldState.h"
#include "ECSManager.h"

class WorldStateManager {
public:
    explicit WorldStateManager(shared_ptr<ECSManager> ecs);
    void StoreNewState(WorldState newState);
    void Rewind(u64 rewindSpeed);
    void Resume();

private:
    shared_ptr<ECSManager> ecs;

    vector<WorldState> worldStates;
    u64 currentFrame { 0 };
    bool isRecording { true };

    void SetFrame(u64 targetFrame);
};


#endif //SUNSET_REVERIES_WORLDSTATEMANAGER_HPP