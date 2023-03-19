//
// Created by gaetz on 19/03/23.
//

#include "Components.h"

#include <utility>
#include "ECSManager.h"

RigidbodyRaycast2D::RigidbodyRaycast2D(u32 entityId, std::shared_ptr<ECSManager> ecsP,
                                       i32 horizontalRaysCountP, i32 verticalRaysCountP,
                                       f32 horizontalRayLength, f32 verticalRayLength,
                                       f32 margin) :
        entityId { entityId },
        verticalRaysCount { verticalRaysCountP }, horizontalRaysCount { horizontalRaysCountP },
        horizontalRayLength {horizontalRayLength}, verticalRayLength { verticalRayLength }, margin { margin },
        ecs { std::move(ecsP) }
{
    if (verticalRaysCount < 2) {
        verticalRaysCount = 2;
    }
    if (horizontalRaysCount < 2) {
        horizontalRaysCount = 2;
    }
    Update();
}

void RigidbodyRaycast2D::Update() {
    attachBody = ecs->GetComponent<Rigidbody2D>(entityId);
    verticalRays.clear();
    verticalRays = std::move(UpdateVerticalRays());
    horizontalRays.clear();
    horizontalRays = std::move(UpdateHorizontalRays());
}