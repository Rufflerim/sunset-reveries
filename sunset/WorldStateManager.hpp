//
// Created by gaetz on 07/03/23.
//

#ifndef SUNSET_REVERIES_WORLD_STATE_MANAGER_HPP
#define SUNSET_REVERIES_WORLD_STATE_MANAGER_HPP

#include "WorldState.hpp"
#include "ECSManager.hpp"


class WorldStateManager {

    enum class RecordingStatus {
        Recording,
        Stop
    };

public:
    explicit WorldStateManager(shared_ptr<ECSManager> ecs);
    void StoreNewState(WorldState newState);
    void Rewind(u64 rewindSpeed);
    void Resume();

    RecordingStatus recordingStatus { RecordingStatus::Recording };
    [[nodiscard]] bool IsRecording() const { return recordingStatus == RecordingStatus::Recording; };

private:
    shared_ptr<ECSManager> ecs;

    vector<WorldState> worldStates;
    u64 currentFrame { 0 };

    void SetFrame(u64 targetFrame);
};


#endif //SUNSET_REVERIES_WORLD_STATE_MANAGER_HPP
