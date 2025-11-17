#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "pipeline.h"
#include "units.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "projection.h"
#include "screen.h"
#include "triangle.h"
#include "math/mathplus.h"
#include "math/matrix4x4.h"
#include "loader.h"

Matrix4x4 *view_matrix = NULL;
ProjectionPlane *view_plane = NULL;

// --- Shaders (your software shaders) ---
void default_shader(VertexShaderEnvironment *env, pipeline_float *vertex)
{
    Vector3 *position = (Vector3 *)vertex;
    /*Vector4 projected_position = matrix4x4_multiply_vector4(
        view_matrix,
        &(Vector4){.x = position->x, .y = position->y, .z = position->z, .w = 1.0f});
    Vector3 final = project_point_in_camera_space_to_plane(view_plane, (Vector3 *)&projected_position);*/

    *env->output =project_3D_point_to_plane(view_plane, position);

    //*env->output = final;
}

const Vector3 light_direction = {0.57735027f, -0.57735027f, 0.57735027f};

void fragment_shader(FragmentShaderEnvironment *env, pipeline_float *vertex)
{
    Vector3 normal = vector3_normalize(((Vector3 *)(vertex) + 1));
    float_unit val = clamp(-vector3_dot(&normal, &light_direction), 0, 1);
    const RGBPixel pixel = {.r = (byte)(255 * val), .g = (byte)(255 * val), .b = (byte)(255 * val)};
    memcpy(env->pixel, &pixel, sizeof(RGBPixel));
}

// --- Camera ---
typedef struct
{
    Vector3 position;
    float yaw;
    float pitch;
} FreeCamera;

Vector3 get_camera_forward(FreeCamera *cam)
{
    float cy = cosf(cam->yaw);
    float sy = sinf(cam->yaw);
    float cp = cosf(cam->pitch);
    float sp = sinf(cam->pitch);
    return (Vector3){sy * cp, sp, cy * cp};
}

Vector3 get_camera_right(FreeCamera *cam)
{
    Vector3 f = get_camera_forward(cam);
    return vector3_normalize(&(Vector3){f.z, 0, -f.x});
}

void update_camera(FreeCamera *cam, GLFWwindow *window, float dt)
{
    const float moveSpeed = 200.0f;
    const float mouseSensitivity = 0.0025f;

    static double lastX = 0, lastY = 0;
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    double deltaX = mouseX - lastX;
    double deltaY = mouseY - lastY;
    lastX = mouseX;
    lastY = mouseY;

    cam->yaw -= deltaX * mouseSensitivity;
    cam->pitch += deltaY * mouseSensitivity;

    if (cam->pitch > 1.5f)
        cam->pitch = 1.5f;
    if (cam->pitch < -1.5f)
        cam->pitch = -1.5f;

    Vector3 forward = view_plane->forward;
    Vector3 right = view_plane->right;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Vector3 scaled = vector3_mul_float(&forward, moveSpeed * dt);
        cam->position = vector3_add(&cam->position, &scaled);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Vector3 scaled = vector3_mul_float(&forward, moveSpeed * dt);
        cam->position = vector3_sub(&cam->position, &scaled);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Vector3 scaled = vector3_mul_float(&right, moveSpeed * dt);
        cam->position = vector3_sub(&cam->position, &scaled);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Vector3 scaled = vector3_mul_float(&right, moveSpeed * dt);
        cam->position = vector3_add(&cam->position, &scaled);
    }
}

void update_view(FreeCamera *cam, ProjectionPlane *plane, Matrix4x4 *view_matrix)
{
    Vector3 forward = get_camera_forward(cam);
    Vector3 up = (Vector3){0, 1, 0};

    plane->origin = cam->position;
    
    forward = vector3_normalize(&forward);
    update_projection_plane(plane, &forward , &up);

    *view_matrix = build_view_matrix(plane);
}

// --- Draw software buffer to screen ---
void draw_buffer_to_screen(ScreenBuffer *buffer, int width, int height)
{
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer->data);
}

// --- Main ---
int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Software Renderer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int screenWidth = 1920;
    int screenHeight = 1080;

    ScreenBuffer buffer = new_screen_buffer(screenWidth, screenHeight, sizeof(RGBPixel));
    ScreenBuffer depth_buffer = new_screen_buffer(screenWidth, screenHeight, sizeof(float));
    float depth_clear = 0;
    fill_screen(&depth_buffer, &depth_clear);

    const float_unit FOV = 120;
    ProjectionPlane plane = create_projection_plane(
        screenWidth, screenHeight,
        &(Vector3){0, 200, -800},
        &(Vector3){0, 0, 1},
        &(Vector3){0, 1, 0},
        degrees_to_radians(FOV / 2));

    Matrix4x4 _view_matrix = build_view_matrix(&plane);
    view_plane = &plane;
    view_matrix = &_view_matrix;

    VertexData data = load_obj("models/car.obj");

    FreeCamera cam = {.position = {0, 200, -800}, .yaw = 0, .pitch = 0};

    double lastTime = glfwGetTime();

    const RGBPixel black = {.r = 0, .g = 0, .b = 0};

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        float dt = (float)(currentTime - lastTime);
        lastTime = currentTime;
        printf("FPS %lf\n", 1 / dt);

        // --- Update ---
        update_view(&cam, &plane, &_view_matrix);
        update_camera(&cam, window, dt);

        fill_screen(&depth_buffer, &depth_clear); // clear depth
        fill_screen(&buffer, &black);
        // --- Render software buffer ---
        render_vertex_data(&buffer, &depth_buffer, default_shader, fragment_shader, &data);

        // --- Draw to screen ---
        glClear(GL_COLOR_BUFFER_BIT);
        glRasterPos2i(-1, -1); // bottom-left
        draw_buffer_to_screen(&buffer, screenWidth, screenHeight);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    free(buffer.data);
    free(depth_buffer.data);
    glfwTerminate();
    return 0;
}
