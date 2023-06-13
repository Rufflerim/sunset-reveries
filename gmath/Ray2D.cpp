//
// Created by gaetz on 13/06/2023.
//

#include "Ray2D.hpp"

namespace gmath {

    Ray2D::Ray2D(Vec2 originP, Vec2 directionP, f32 lengthP) :
        origin {originP},
        direction {directionP},
        length {lengthP}
    {}

    bool RayVsRect2D(const Vec2 &rayOrigin, const Vec2 &rayDir, const Rect &targetRect, Vec2 &contactPoint,
                     Vec2 &contactNormal, float &nearContactTime, std::array<Vec2, 2> &probableContactPoints) {

        /*
        * The t in the P(t) = P + D.t
        * Where t is the parametric variable to plot the near collision point using the parametric line equation (P(t) = P + D.t)
        * Where P is the Position Vector of the Ray and D is the Direction Vector of the Ray
        */
        float tHitNear{0.0f};

        /*
        * Calculate intersection points with Rect bounding axes
        * Parametric 't' for Near (X,Y) and Far (X,Y)
        */
        float deltaT1X = targetRect.x - rayOrigin.x;
        float tHitNearX = (deltaT1X / rayDir.x);

        float deltaT1Y = targetRect.y - rayOrigin.y;
        float tHitNearY = (deltaT1Y / rayDir.y);

        float deltaT2X = targetRect.x + targetRect.width - rayOrigin.x;
        float tHitFarX = (deltaT2X / rayDir.x);

        float deltaT2Y = targetRect.y + targetRect.height - rayOrigin.y;
        float tHitFarY = (deltaT2Y / rayDir.y);

        /*
        * Sort the distances to maintain Affine uniformity
        * i.e. sort the near and far axes of the Rect in the appropriate order from the POV of ray_origin
        */
        if (tHitNearX > tHitFarX) std::swap(tHitNearX, tHitFarX);
        if (tHitNearY > tHitFarY) std::swap(tHitNearY, tHitFarY);

        // As there is no chance of collision i.e. the parametric line cannot pass through the Rect the probable points are empty
        probableContactPoints[0] = Vec2{0, 0};
        probableContactPoints[1] = Vec2{0, 0};

        /*
        * Check the order of the near and far points
        * if they satisfy the below condition the line will pass through the Rect (It didn't yet)
        * if not return out of the function as it will not pass through
        */
        if (!(tHitNearX < tHitFarY && tHitNearY < tHitFarX)) return false;

        /*
        * If the parametric line passes through the Rect calculate the parametric 't'
        * the 't' should be such that it must lie on both the Line/Ray and the Rect
        * Use the condition below to calculate the 'tNear' and 'tFar' that gives the near and far collision parametric t
        */
        nearContactTime = std::max(tHitNearX, tHitNearY);
        f32 tHitFar = std::min(tHitFarX, tHitFarY);

        // Use the parametric t values calculated above and substitute them in the parametric equation to get the near and far contact points
        f32 hitNearXPosition = rayOrigin.x + (rayDir.x * (nearContactTime));
        f32 hitNearYPosition = rayOrigin.y + (rayDir.y * (nearContactTime));

        f32 hitFarXPosition = rayOrigin.x + (rayDir.x * tHitFar);
        f32 hitFarYPosition = rayOrigin.y + (rayDir.y * tHitFar);

        // Generate Vectors using the near and far collision points
        auto nearHitVector = Vec2{hitNearXPosition, hitNearYPosition};
        auto farHitVector = Vec2{hitFarXPosition, hitFarYPosition};
        // Since we are sure that the line will pass through the Rect update the probable near and far points
        probableContactPoints[0] = nearHitVector;
        probableContactPoints[1] = farHitVector;

        /*
        * Check whether the parametric 't' values are withing certain bounds to guarantee that the probable collision has actually occurred
        * i.e. If the below conditions are true only then the Ray has passed through the Rect
        * if not it still can pass, but it did not yet
        */
        if (tHitFar < 0 || tHitNear > 1) return false;

        // Now Update the actual contact point
        contactPoint = Vec2{hitNearXPosition, hitNearYPosition};

        // Update the contact normal of the Ray with the Rect surface
        if (tHitNearX > tHitNearY) {
            if (rayDir.x < 0) contactNormal = Vec2{1, 0};
            else contactNormal = Vec2{-1, 0};
        } else if (tHitNearX < tHitNearY) {
            if (rayDir.y < 0) contactNormal = Vec2{0, 1};
            else contactNormal = Vec2{0, -1};
        }
        // Since the contact has definitely occured return true
        return true;
    }

}