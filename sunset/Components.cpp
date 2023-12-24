//
// Created by gaetz on 19/03/23.
//

/*

#include "Components.hpp"

#include <utility>
#include "ECSManager.hpp"
#include <cmath>

RigidbodyRaycast2D::RigidbodyRaycast2D(u64 entityId, sptr<ECSManager> ecsP,
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

void RigidbodyRaycast2D::SetRayDirection(Ray2DDirection direction) {
    switch (direction) {
        case Ray2DDirection::Left:
            if (currentHorizontalDirection == Ray2DDirection::Left) return;
            currentHorizontalDirection = Ray2DDirection::Left;
            break;
        case Ray2DDirection::Right:
            if (currentHorizontalDirection == Ray2DDirection::Right) return;
            currentHorizontalDirection = Ray2DDirection::Right;
            break;
        case Ray2DDirection::Down:
            if (currentVerticalDirection == Ray2DDirection::Down) return;
            currentVerticalDirection = Ray2DDirection::Down;
            break;
        case Ray2DDirection::Up:
            if (currentVerticalDirection == Ray2DDirection::Up) return;
            currentVerticalDirection = Ray2DDirection::Up;
            break;
    }
}

vector<Ray2D> RigidbodyRaycast2D::UpdateHorizontalRays() {
    vector<Ray2D> rays;
    const Vec2 startPosition { attachBody.pos + Vec2 { attachBody.boundingBox.x, attachBody.boundingBox.y } };
    const Vec2 endPosition = startPosition + Vec2 { 0.0f, attachBody.boundingBox.height };
    const Vec2 offset = (endPosition - startPosition) / static_cast<f32>(horizontalRaysCount - 1);
    const Vec2 indent { 0.0f, margin };
    if (currentHorizontalDirection == Ray2DDirection::Left) {
        for (i32 i = 0; i < horizontalRaysCount; ++i) {
            rays.emplace_back(
                    startPosition + offset * static_cast<f32>(i) + Vec2 { margin, 0.0f }
                    + (i == 0 ? indent : Vec2 {0, 0})
                    + (i == horizontalRaysCount - 1 ? -1.0f * indent : Vec2 {0, 0}),
                    Vec2 { -1, 0 },
                    horizontalRayLength + margin);
        }
    } else {
        for (i32 i = 0; i < horizontalRaysCount; ++i) {
            rays.emplace_back(
                    startPosition + offset * static_cast<f32>(i) - Vec2 {margin, 0.0f }
                    + Vec2 { attachBody.boundingBox.width, 0.0f }
                    + (i == 0 ? indent : Vec2 {0, 0})
                    + (i == horizontalRaysCount - 1 ? -1.0f * indent : Vec2 {0, 0}),
                    Vec2 { 1, 0 },
                    horizontalRayLength + margin);
        }
    }
    return rays;
}

vector<Ray2D> RigidbodyRaycast2D::UpdateVerticalRays() {
    vector<Ray2D> rays;
    const Vec2 startPosition { attachBody.pos + Vec2 { attachBody.boundingBox.x, attachBody.boundingBox.y } };
    const Vec2 endPosition = startPosition + Vec2 { attachBody.boundingBox.width, 0.0f };
    const Vec2 offset = (endPosition - startPosition) / static_cast<f32>(verticalRaysCount - 1);
    const Vec2 indent {margin, 0.0f };
    if (currentVerticalDirection == Ray2DDirection::Down) {
        for (i32 i = 0; i < verticalRaysCount; ++i) {
            rays.emplace_back(
                    startPosition + offset * static_cast<f32>(i) - Vec2 { 0.0f, margin }
                    + Vec2 { 0.0f, attachBody.boundingBox.height }
                    + (i == 0 ? indent : Vec2 {0, 0})
                    + (i == verticalRaysCount - 1 ? -1.0f * indent : Vec2 {0, 0}),
                    Vec2 { 0, 1 },
                    verticalRayLength + margin);
        }
    } else {
        for (i32 i = 0; i < verticalRaysCount; ++i) {
            rays.emplace_back(
                    startPosition + offset * static_cast<f32>(i) + Vec2 { 0.0f, margin }
                    + (i == 0 ? indent : Vec2 {0, 0})
                    + (i == verticalRaysCount - 1 ? -1.0f * indent : Vec2 {0, 0}),
                    Vec2 { 0, -1 },
                    verticalRayLength + margin);
        }
    }
    return rays;
}

void Weapon::Update(f32 dt) {
    cooldownTimer += dt;
}

void Weapon::Shoot() {
    if (cooldownTimer >= cooldown) {
        cooldownTimer = 0;
        for (i32 i = 0; i < projectilePerShoot; ++i) {
            ShootOnce();
        }
    }
}

void Weapon::ShootOnce() const {
    f32 shootAngle { 0.0f };
    shootAngle += angle;
    switch (currentOrientation) {
        case CharacterOrientation::Right:
            shootAngle += static_cast<f32>(GetRandomValue(-angularSpreadDegree, angularSpreadDegree));
            break;
        case CharacterOrientation::Left:
            shootAngle = 180.0f;
            shootAngle += static_cast<f32>(GetRandomValue(-angularSpreadDegree, angularSpreadDegree));
            break;
        case CharacterOrientation::Top:
            shootAngle = 90.0f;
            shootAngle += static_cast<f32>(GetRandomValue(-angularSpreadDegree, angularSpreadDegree));
            break;
        case CharacterOrientation::Bottom:
            shootAngle = 270.0f;
            shootAngle += static_cast<f32>(GetRandomValue(-angularSpreadDegree, angularSpreadDegree));
            break;
    }
    const f32 radianShootAngle = shootAngle * PI / 360.0f;

    projectileBuffer->CreateProjectile(entityId, radianShootAngle);
}

 */