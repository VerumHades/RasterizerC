#ifndef PROJECTION_H
#define PROJECTION_H

#include "math/vector3.h"
#include "math/vector2.h"
#include "math/matrix4x4.h"

typedef struct ProjectionPlane {
    Vector3 origin, forward, up, right;
    float_unit near, width, height;
} ProjectionPlane;

ProjectionPlane create_projection_plane(float_unit width, float_unit height, Vector3* origin, Vector3* forward, Vector3* up, float_unit fov_rad);
void update_projection_plane(ProjectionPlane* plane, Vector3* forward, Vector3* up);

Vector3 project_3D_point_to_plane(ProjectionPlane* plane, Vector3* point);

Vector3 device_coordinates_to_screen(float_unit width, float_unit height, Vector3* coordinates);
Matrix4x4 build_view_matrix(ProjectionPlane* plane);
Vector3 project_point_in_camera_space_to_plane(ProjectionPlane* plane, Vector3* point);

#endif // PROJECTION_H
