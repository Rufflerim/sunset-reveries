//
// Created by gaetz on 07/03/23.
//

#include "WorldStateManager.hpp"

WorldStateManager::WorldStateManager(shared_ptr<ECSManager> ecs) :
    ecs { std::move(ecs) } {
    // TODO: Reserve for more than 5 minutes. Or implement a level maximum time?
    worldStates.reserve(3600 * 5);
}

void WorldStateManager::StoreNewState(WorldState newState) {
    if (!IsRecording()) return;
    currentFrame = worldStates.size();
    newState.currentFrame = currentFrame;
    worldStates.push_back(std::move(newState));
}

void WorldStateManager::SetFrame(u64 targetFrame) {
    if (targetFrame <= 0) {
        return;
    }

    ecs->SetWorldState(worldStates.at(targetFrame));
    currentFrame = targetFrame;
}

void WorldStateManager::Rewind(u64 rewindSpeed) {
    if (startRewindFrame == 0) {
        // Beware, this value has to be reset to start a new recording
        startRewindFrame = currentFrame;
    }
    recordingStatus = RecordingStatus::Stop;
    u32 targetFrame = currentFrame - rewindSpeed;
    if (rewindSpeed > currentFrame) targetFrame = 0;
    SetFrame(targetFrame);
}

void WorldStateManager::Resume() {
    // Save player ghost as a new entity
    u32 newPlayerGhostId = ecs->CreateEntity();

    // Add a recording component on this new entity, which contains recorded data fo each frame
    ecs->CreateTransform2DComponent(newPlayerGhostId);
    ecs->CreateSpriteComponent(newPlayerGhostId, "ghost");
    const auto& ghostTexture = AssetsManager::GetTexture("ghost");
    ecs->CreateRigidbody2DComponent(newPlayerGhostId, { 0, 0 },
                                    {0, 0, static_cast<float>(ghostTexture.width), static_cast<float>(ghostTexture.height)},
                                    false, true);
    u32 formerEntityId = 0; // TODO Check player id
    ecs->CreateReplayComponent(newPlayerGhostId, formerEntityId, currentFrame, startRewindFrame);
    Replay& replay = ecs->GetComponent<Replay>(newPlayerGhostId);

    // Fill the recording component with all data for transform, rigidbody and sprite
    for (auto itr = worldStates.begin() + currentFrame; itr != worldStates.begin() + startRewindFrame; ++itr) {
        replay.transforms.push_back(itr->GetWorldStateComponent<Transform2D>(formerEntityId));
        replay.sprites.push_back(itr->GetWorldStateComponent<Sprite>(formerEntityId));
        replay.bodies.push_back(itr->GetWorldStateComponent<Rigidbody2D>(formerEntityId));
        replay.transforms.back().entityId = newPlayerGhostId;
        replay.sprites.back().entityId = newPlayerGhostId;
        replay.sprites.back().texName = "ghost";
        replay.sprites.back().tex = ghostTexture;
        replay.bodies.back().entityId = newPlayerGhostId;
    }

    // Remove rewinded frames
    size_t length = worldStates.size();
    for (int i = 0; i < length - currentFrame; ++i) {
        worldStates.pop_back();
    }

    // Reset and return
    startRewindFrame = 0;
    recordingStatus = RecordingStatus::Recording;
}
