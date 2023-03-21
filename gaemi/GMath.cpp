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

bool RayVsRect2D(const Vector2& ray_origin, const Vector2& ray_dir, const Rectangle& targetRect, Vector2& contact_point, Vector2& contact_normal, float& near_contact_time, std::array<Vector2, 2>& probableContactPoints){

    /*
    * The t in the P(t) = P + D.t
    * Where t is the parametric variable to plot the near collison point using the parametric line equation (P(t) = P + D.t)
    * Where P is the Position Vector of the Ray and D is the Direciton Vector of the Ray
    */
    float t_hit_near;

    /*
    * Calculate intersection points with rectangle bounding axes
    * Parametric 't' for Near (X,Y) and Far (X,Y)
    */
    float delta_t1_X = targetRect.x - ray_origin.x;
    float t_hit_near_X = (delta_t1_X / ray_dir.x);

    float delta_t1_Y = targetRect.y - ray_origin.y;
    float t_hit_near_Y = (delta_t1_Y / ray_dir.y);

    float delta_t2_X = targetRect.x + targetRect.width - ray_origin.x;
    float t_hit_far_X = (delta_t2_X / ray_dir.x);

    float delta_t2_Y = targetRect.y + targetRect.height - ray_origin.y;
    float t_hit_far_Y = (delta_t2_Y / ray_dir.y);

    /*
    * Sort the distances to maintain Affine uniformity
    * i.e. sort the near and far axes of the rectangle in the appropritate order from the POV of ray_origin
    */
    if (t_hit_near_X > t_hit_far_X) SwapFloatValue(&t_hit_near_X, &t_hit_far_X);
    if (t_hit_near_Y > t_hit_far_Y) SwapFloatValue(&t_hit_near_Y, &t_hit_far_Y);

    // As there is no chance of collision i.e the paramteric line cannot pass throguh the rectangle the probable points are empty
    probableContactPoints[0] = (Vector2){0, 0};
    probableContactPoints[1] = (Vector2){0, 0};

    /*
    * Check the order of the near and far points
    * if they satisfy the below condition the line will pass through the rectanle (It didn't yet)
    * if not return out of the function as it will not pass through
    */
    if(!(t_hit_near_X < t_hit_far_Y && t_hit_near_Y < t_hit_far_X)) return false;

    /*
    * If the parametric line passes through the rectangle calculate the parametric 't'
    * the 't' should be such that it must lie on both the Line/Ray and the Rectangle
    * Use the condition below to calculate the 'tnear' and 'tfar' that gives the near and far collison parametric t
    */
    near_contact_time = std::max(t_hit_near_X, t_hit_near_Y);
    float t_hit_far = std::min(t_hit_far_X, t_hit_far_Y);

    // Use the parametric t values calculated above and subsitute them in the parametric equation to get the near and far contact points
    float Hit_Near_X_Position = ray_origin.x + (ray_dir.x * (near_contact_time));
    float Hit_Near_Y_Position = ray_origin.y + (ray_dir.y * (near_contact_time));

    float Hit_Far_X_Position = ray_origin.x + (ray_dir.x * t_hit_far);
    float Hit_Far_Y_Position = ray_origin.y + (ray_dir.y * t_hit_far);

    // Generate Vectors using the near and far collision points
    Vector2 Near_Hit_Vector = (Vector2){Hit_Near_X_Position, Hit_Near_Y_Position};
    Vector2 Far_Hit_Vector = (Vector2){Hit_Far_X_Position, Hit_Far_Y_Position};
    // Since we are sure that the line will pass through the rectanle upadte the probable near and far points
    probableContactPoints[0] = Near_Hit_Vector;
    probableContactPoints[1] = Far_Hit_Vector;

    /*
    * Check wether the parametric 't' values are withing certain bounds to guarantee that the probable collision has actually occured
    * i.e. If the below conditions are true only then the Ray has passed through the Rectangle
    * if not it still can pass but it did not yet
    */
    if(t_hit_far < 0 || t_hit_near > 1) return false;

    // Now Update the actual contact point
    contact_point = (Vector2){Hit_Near_X_Position, Hit_Near_Y_Position};

    // Update the contact normal of the Ray with the Rectangle surface
    if(t_hit_near_X > t_hit_near_Y){
        if(ray_dir.x < 0) contact_normal = (Vector2){1, 0};
        else contact_normal = (Vector2){-1, 0};
    }
    else if(t_hit_near_X < t_hit_near_Y){
        if(ray_dir.y < 0) contact_normal = (Vector2){0, 1};
        else  contact_normal = (Vector2){0, -1};
    }
    // Since the contact has definitely occured return true
    return true;
}