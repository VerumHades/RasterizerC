#include "pipeline.h"
#include "projection.h"
#include "screen.h"
#include "triangle.h"
#include <string.h>
#include "math/mathplus.h"

int fill_triangle(
    ScreenBuffer *buffer,
    ScreenBuffer *depth_buffer,
    Triangle3 *triangle,
    FragmentShader fragment_shader,
    VertexData *vertex_data,
    pipeline_float *first_vertex)
{
    BoundingBox box = get_triangle_bounding_box(triangle);
    int x_start = clamp(box.min.x, 0, buffer->width);
    int x_end = clamp(box.max.x, 0, buffer->width);
    int y_start = clamp(box.min.y, 0, buffer->height);
    int y_end = clamp(box.max.y, 0, buffer->height);

    ComputedTriangleData data = compute_triangle_normals(triangle);

    FragmentShaderEnvironment env = {0};
    TriangleRenderResult result = {0};
    float *uvs = result.uvs;

    const int FLIP_Y = 0;

    float temp_store[MAX_VERTEX_SIZE];

    for (int y = y_start; y <= y_end; y++)
    {
        for (int x = x_start; x <= x_end; x++)
        {
            Vector2 point = {.x = (float)x, .y = (float)y};

            if (point_in_triangle_precomputed(triangle, &data, &point, &result))
            {
                if(!result.clockwise)
                    return;
                

                int index = coordinates_to_buffer_index(buffer, x, FLIP_Y ? buffer->height - y : y);
                if (index >= buffer->width * buffer->height)
                    continue;

                float *buffer_depth = &depth_buffer->data[index * depth_buffer->pixel_size];
                float current_depth = triangle->points[0].z * uvs[0] + triangle->points[1].z * uvs[1] + triangle->points[2].z * uvs[2];
                
                //printf("%f \n", buffer_depth);
                //if(current_depth <= 0) continue;
                if (current_depth <= *buffer_depth)
                    continue;

                *buffer_depth = current_depth;

                for (size_t i = 0; i < vertex_data->vertex_size; i++)
                {
                    temp_store[i] = first_vertex[i] * uvs[0] +
                                    first_vertex[vertex_data->vertex_size + i] * uvs[1] +
                                    first_vertex[vertex_data->vertex_size * 2 + i] * uvs[2];
                }

                env.pixel = buffer->data + index * buffer->pixel_size;
                fragment_shader(&env, temp_store);
            }
        }
    }

    return 0;
}

int render_vertex_data(ScreenBuffer *buffer, ScreenBuffer *depth_buffer, VertexShader vertex_shader, FragmentShader fragment_shader, VertexData *vertex_data)
{
    size_t vertex_count = vertex_data->size / vertex_data->vertex_size;
    if (vertex_count % 3 != 0)
        return INVALID_VERTEX_DATA;

    for (size_t i = 0; i < vertex_data->size; i += vertex_data->vertex_size * 3)
    {
        Triangle3 projected_triangle = {0};

        for (int j = 0; j < 3; j++)
        {
            pipeline_float *vertex = &vertex_data->data[i + j * vertex_data->vertex_size];

            vertex_shader(&(VertexShaderEnvironment){
                              .output = &projected_triangle.points[j]},
                          vertex);

            projected_triangle.points[j] = device_coordinates_to_screen(buffer->width, buffer->height, &projected_triangle.points[j]);
        }

        fill_triangle(
            buffer,
            depth_buffer,
            &projected_triangle,
            fragment_shader,
            vertex_data,
            &vertex_data->data[i]);
    }

    return SUCCESS;
}