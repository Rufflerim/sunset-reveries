//
// Created by gaetz on 07/03/23.
//

#include "WorldStateManager.hpp"

WorldStateManager::WorldStateManager(shared_ptr<ECSManager> ecs) :
    ecs { std::move(ecs) } {

}

void WorldStateManager::StoreNewState(WorldState newState) {
    if (!IsRecording()) return;
    currentFrame = worldStates.size();
    worldStates.push_back(std::move(newState));
}

void WorldStateManager::SetFrame(u64 targetFrame) {
    if (targetFrame <= 0) {
        return;
    }

    ecs->SetWorldState(worldStates.at(targetFrame));
    for (int i = 0; i < currentFrame - targetFrame; ++i) {
        worldStates.pop_back();
    }
    currentFrame = targetFrame;
}

void WorldStateManager::Rewind(u64 rewindSpeed) {
    recordingStatus = RecordingStatus::Stop;
    u64 targetFrame = currentFrame - rewindSpeed;
    if (rewindSpeed > currentFrame) targetFrame = 0;
    SetFrame(targetFrame);
}

void WorldStateManager::Resume() {
    recordingStatus = RecordingStatus::Recording;
}
