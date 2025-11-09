#ifndef PROJECTION_H
#define PROJECTION_H

#include "vector3.h"
#include "vector2.h"
#include "matrix4x4.h"

typedef struct ProjectionPlane {
    Vector3 origin, forward, up, right;
    float_unit near, width, height;
} ProjectionPlane;

typedef struct Camera {
    ProjectionPlane plane;
    Matrix4x4 view_matrix;
} Camera;

ProjectionPlane create_projection_plane(float_unit width, float_unit height, Vector3* origin, Vector3* forward, Vector3* up, float_unit fov_rad);
Camera create_camera(ProjectionPlane* plane);

Vector2 project_3D_point_to_plane(ProjectionPlane* plane, Vector3* point);

Vector2 device_coordinates_to_screen(float_unit width, float_unit height, Vector2* coordinates);
Matrix4x4 build_view_matrix(ProjectionPlane* plane);
Vector2 project_point_in_camera_space_to_plane(ProjectionPlane* plane, Vector3* point);

#endif // PROJECTION_H
