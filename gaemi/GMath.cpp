//
// Created by gaetz on 12/03/2023.
//

#include "GMath.h"

Vector2 operator+(Vector2 lh, Vector2 rh) {
    Vector2 res { lh.x + rh.x, lh.y + rh.y };
    return res;
}

Vector2 operator-(Vector2 lh, Vector2 rh) {
    Vector2 res { lh.x - rh.x, lh.y - rh.y };
    return res;
}

Vector2 operator*(Vector2 v, f32 f) {
    Vector2 res { v.x * f, v.y * f };
    return res;
}

Vector2 operator*(f32 f, Vector2 v) {
    Vector2 res { v.x * f, v.y * f };
    return res;
}

Vector2 operator/(Vector2 v, f32 f) {
    Vector2 res { v.x / f, v.y / f };
    return res;
}

Vector2 operator/(f32 f, Vector2 v) {
    Vector2 res { v.x / f, v.y / f };
    return res;
}

void SwapFloatValue(float *a, float *b){
    float t = *a;
    *a = *b;
    *b = t;
}

bool RayVsRect2D(const Vector2& ray_origin, const Vector2& rayDir, const Rectangle& targetRect, Vector2& contactPoint, Vector2& contactNormal, float& near_contact_time, std::array<Vector2, 2>& probableContactPoints){

    /*
    * The t in the P(t) = P + D.t
    * Where t is the parametric variable to plot the near collison point using the parametric line equation (P(t) = P + D.t)
    * Where P is the Position Vector of the Ray and D is the Direciton Vector of the Ray
    */
    float tHitNear { 0.0f };

    /*
    * Calculate intersection points with rectangle bounding axes
    * Parametric 't' for Near (X,Y) and Far (X,Y)
    */
    float deltaT1X = targetRect.x - ray_origin.x;
    float tHitNearX = (deltaT1X / rayDir.x);

    float deltaT1Y = targetRect.y - ray_origin.y;
    float tHitNearY = (deltaT1Y / rayDir.y);

    float deltaT2X = targetRect.x + targetRect.width - ray_origin.x;
    float tHitFarX = (deltaT2X / rayDir.x);

    float deltaT2Y = targetRect.y + targetRect.height - ray_origin.y;
    float tHitFarY = (deltaT2Y / rayDir.y);

    /*
    * Sort the distances to maintain Affine uniformity
    * i.e. sort the near and far axes of the rectangle in the appropritate order from the POV of ray_origin
    */
    if (tHitNearX > tHitFarX) SwapFloatValue(&tHitNearX, &tHitFarX);
    if (tHitNearY > tHitFarY) SwapFloatValue(&tHitNearY, &tHitFarY);

    // As there is no chance of collision i.e the paramteric line cannot pass throguh the rectangle the probable points are empty
    probableContactPoints[0] = Vector2 {0, 0};
    probableContactPoints[1] = Vector2 {0, 0};

    /*
    * Check the order of the near and far points
    * if they satisfy the below condition the line will pass through the rectanle (It didn't yet)
    * if not return out of the function as it will not pass through
    */
    if(!(tHitNearX < tHitFarY && tHitNearY < tHitFarX)) return false;

    /*
    * If the parametric line passes through the rectangle calculate the parametric 't'
    * the 't' should be such that it must lie on both the Line/Ray and the Rectangle
    * Use the condition below to calculate the 'tnear' and 'tfar' that gives the near and far collison parametric t
    */
    near_contact_time = std::max(tHitNearX, tHitNearY);
    float t_hit_far = std::min(tHitFarX, tHitFarY);

    // Use the parametric t values calculated above and subsitute them in the parametric equation to get the near and far contact points
    float hitNearXPosition = ray_origin.x + (rayDir.x * (near_contact_time));
    float hitNearYPosition = ray_origin.y + (rayDir.y * (near_contact_time));

    float hitFarXPosition = ray_origin.x + (rayDir.x * t_hit_far);
    float hitFarYPosition = ray_origin.y + (rayDir.y * t_hit_far);

    // Generate Vectors using the near and far collision points
    auto nearHitVector = Vector2 { hitNearXPosition, hitNearYPosition};
    auto farHitVector = Vector2 { hitFarXPosition, hitFarYPosition};
    // Since we are sure that the line will pass through the rectanle upadte the probable near and far points
    probableContactPoints[0] = nearHitVector;
    probableContactPoints[1] = farHitVector;

    /*
    * Check wether the parametric 't' values are withing certain bounds to guarantee that the probable collision has actually occured
    * i.e. If the below conditions are true only then the Ray has passed through the Rectangle
    * if not it still can pass but it did not yet
    */
    if(t_hit_far < 0 || tHitNear > 1) return false;

    // Now Update the actual contact point
    contactPoint = Vector2 { hitNearXPosition, hitNearYPosition};

    // Update the contact normal of the Ray with the Rectangle surface
    if(tHitNearX > tHitNearY){
        if(rayDir.x < 0) contactNormal = Vector2 { 1, 0};
        else contactNormal = Vector2 { -1, 0};
    }
    else if(tHitNearX < tHitNearY){
        if(rayDir.y < 0) contactNormal = Vector2 { 0, 1};
        else contactNormal = Vector2{ 0, -1};
    }
    // Since the contact has definitely occured return true
    return true;
}

f32 easeOutExpo(f32 t) {
    return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
}