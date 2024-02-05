//
// Created by gaetz on 17/06/2023.
//

/*

#include "WorldChanger.hpp"
#include "WorldState.hpp"

void WorldChanger::PushPositionChange(PositionChange positionChange) {
    positionChanges.emplace_back(positionChange);
}

void WorldChanger::PushPlayerChange(PlayerChange playerChange) {
    playerChanges.emplace_back(playerChange);
}

void WorldChanger::ClearFrameChanges() {
    positionChanges.clear();
    playerChanges.clear();
}

WorldState WorldChanger::UpdateWorld(WorldState worldState) {
    // Player
    for (auto playerChange: playerChanges) {
        auto& body = worldState.GetWorldStateComponent<Rigidbody2D>(playerChange.entityId);
        body.playerAcceleration = playerChange.velocityDelta;
    }

    // Position
    for (const auto positionChange: positionChanges) {
        auto &transform = worldState.GetWorldStateComponent<Transform2D>(positionChange.entityId);
        auto &body = worldState.GetWorldStateComponent<Rigidbody2D>(positionChange.entityId);

        // Manage moving ground
        body.isGrounded = positionChange.isGrounded;
        body.groundVelocity = positionChange.groundVelocity;
        if (body.playerAcceleration.y < 0) {
            body.isGrounded = false;
            // Compensate ground move for more control
            body.playerAcceleration = body.playerAcceleration - body.groundVelocity;
        }

        // Manage velocity and player movement
        body.velocity = body.velocity + positionChange.velocityDelta + body.playerAcceleration;
        body.playerAcceleration = Vec2 {0, 0 };

        if (positionChange.stopVelocityX) {
            body.velocity.x = 0;
        }
        if (positionChange.stopVelocityY && body.isGrounded && (body.velocity.y > 0 || body.velocity.y < 0)) {
            body.velocity.y = 0;
        }

        transform.pos = transform.pos + body.velocity * positionChange.dt + positionChange.groundVelocity * positionChange.dt;
        if (positionChange.positionXFixAfterCollision != -1.0f) {
            transform.pos.x = positionChange.positionXFixAfterCollision;
        }
        if (positionChange.positionYFixAfterCollision != -1.0f && body.isGrounded) {
            transform.pos.y = positionChange.positionYFixAfterCollision;
        }
        body.pos = transform.pos;
    }

    return worldState;
}

 */