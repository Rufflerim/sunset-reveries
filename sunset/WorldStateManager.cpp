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
    if (startRewindFrame == 0) {
        startRewindFrame = currentFrame;
    }
    recordingStatus = RecordingStatus::Stop;
    u32 targetFrame = currentFrame - rewindSpeed;
    if (rewindSpeed > currentFrame) targetFrame = 0;
    SetFrame(targetFrame);
}

void WorldStateManager::Resume() {
    // Save player ghost as a new entity
    //u32 newPlayerGhostId = ecs->CreateEntity();
    // Add a recording component on this new entity, which contains recorded data fo each frame
    for (auto itr = worldStates.begin() + currentFrame; itr != worldStates.begin() + startRewindFrame; ++itr) {
        // Fill the recording component with all data for transform, rigidbody and sprite
    }
    // Remove rewinded frames
    size_t length = worldStates.size();
    for (int i = 0; i < length - currentFrame; ++i) {
        worldStates.pop_back();
    }

    recordingStatus = RecordingStatus::Recording;
}
