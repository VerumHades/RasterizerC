#include "projection.h"
#include <math.h>

Vector3 project_3D_point_to_plane(ProjectionPlane* plane, Vector3* point){
    Vector3 projection_vector = vector3_from_a_to_b(&plane->origin, point);
    
    float_unit length_projected_to_normal = vector3_dot(&projection_vector, &plane->forward);
    float_unit depth = plane->near / length_projected_to_normal;

    Vector3 result = {0};
    if(depth < 0) return result;
    
    Vector3 projected_vector = vector3_scale(&projection_vector, depth);
    
    float_unit projected_up_length = vector3_dot(&projected_vector, &plane->up);
    float_unit projected_right_length = vector3_dot(&projected_vector, &plane->right);

    result.x = projected_right_length / (plane->width / 2.0f);
    result.y = projected_up_length / (plane->height / 2.0f);
    result.z = depth;

    return result;
}


Matrix4x4 build_view_matrix(ProjectionPlane* plane){
    Matrix4x4 view = {0};

    // Camera axes
    Vector3 f = plane->forward; // forward
    Vector3 r = plane->right;   // right
    Vector3 u = plane->up;      // up
    Vector3 o = plane->origin;  // camera position

    // First row: right axis
    view.m[0][0] = r.x;
    view.m[0][1] = r.y;
    view.m[0][2] = r.z;
    view.m[0][3] = -vector3_dot(&r, &o); // translation

    // Second row: up axis
    view.m[1][0] = u.x;
    view.m[1][1] = u.y;
    view.m[1][2] = u.z;
    view.m[1][3] = -vector3_dot(&u, &o);

    // Third row: forward axis (camera looks along +Z by convention)
    view.m[2][0] = f.x;
    view.m[2][1] = f.y;
    view.m[2][2] = f.z;
    view.m[2][3] = -vector3_dot(&f, &o);

    // Fourth row: homogeneous
    view.m[3][0] = 0.0f;
    view.m[3][1] = 0.0f;
    view.m[3][2] = 0.0f;
    view.m[3][3] = 1.0f;

    return view;
}

Vector3 project_point_in_camera_space_to_plane(ProjectionPlane* plane, Vector3* point){
    // Compute scale factor along the camera's forward axis
    float_unit depth = plane->near / vector3_dot(point, &plane->forward);
    
    Vector3 result = {0};
    if(depth < 0) return result;
    

    Vector3 projected = vector3_scale(point, depth);
    // Normalize by half of plane width/height
    result.x = projected.x / (plane->width / 2.0f);
    result.y = projected.y / (plane->height / 2.0f);
    result.z = depth;

    return result;
}

Vector3 device_coordinates_to_screen(float_unit width, float_unit height, Vector3* coordinates){
    return (Vector3) {
        .x = (coordinates->x + 1.0) / 2 * width,
        .y = (coordinates->y + 1.0) / 2 * height,
        .z = coordinates->z
    };
}

void update_projection_plane(ProjectionPlane* plane, Vector3* forward, Vector3* up){
    plane->forward = vector3_normalize(forward);
    plane->up = vector3_normalize(up);
    plane->right = vector3_cross(forward, up);
    plane->up = vector3_cross(&plane->right, &plane->forward);
}

ProjectionPlane create_projection_plane(float_unit width, float_unit height, Vector3* origin, Vector3* forward, Vector3* up, float_unit fov_rad){
    ProjectionPlane plane = {0};
    
    plane.origin = *origin;
    plane.forward = vector3_normalize(forward);
    plane.up = vector3_normalize(up);
    plane.right = vector3_cross(forward, up);

    plane.near = (width / 2) / tan(fov_rad);

    plane.width = width;
    plane.height = height;


    return plane;
}