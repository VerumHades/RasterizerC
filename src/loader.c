#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loader.h"

#include <math.h>
// Returns index of v
static int parse_pos_index(const char *token)
{
    return atoi(token);
}

// Returns index of vn (or -1 if no normal)
static int parse_normal_index(const char *token)
{
    // find first slash
    const char *s = strchr(token, '/');
    if (!s)
        return -1;

    // find second slash
    s = strchr(s + 1, '/');
    if (!s)
        return -1;

    // atoi will read the number after second slash
    return atoi(s + 1);
}
// -------------------------------------------
// Simple OBJ Loader (supports v, vn, f)
// -------------------------------------------
VertexData load_obj(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "Cannot open OBJ: %s\n", filename);
        return (VertexData){0};
    }

    float (*positions)[3] = NULL;
    float (*normals)[3] = NULL;
    float (*colors)[3] = NULL;
    size_t pos_count = 0, nor_count = 0, col_count = 0;

    // final vertex buffer
    float *vertex_buffer = NULL;
    size_t vertex_count = 0;

    char line[256];
    while (fgets(line, sizeof(line), f))
    {

        // -----------------------------------------
        // Read Vertex Positions
        // -----------------------------------------
        if (strncmp(line, "v ", 2) == 0)
        {
            positions = (float (*)[3])realloc(positions, sizeof(float[3]) * (pos_count + 1));
            sscanf(line, "v %f %f %f",
                   &positions[pos_count][0],
                   &positions[pos_count][1],
                   &positions[pos_count][2]);
            pos_count++;

            // -----------------------------------------
            // Read Vertex Colors (optional extension)
            // Format: "vc r g b"
            // -----------------------------------------
        }
        else if (strncmp(line, "vc ", 3) == 0)
        {
            colors = (float (*)[3])realloc(colors, sizeof(float[3]) * (col_count + 1));
            sscanf(line, "vc %f %f %f",
                   &colors[col_count][0],
                   &colors[col_count][1],
                   &colors[col_count][2]);
            col_count++;

            // -----------------------------------------
            // Read Normals
            // -----------------------------------------
        }
        else if (strncmp(line, "vn ", 3) == 0)
        {
            normals = (float (*)[3])realloc(normals, sizeof(float[3]) * (nor_count + 1));
            sscanf(line, "vn %f %f %f",
                   &normals[nor_count][0],
                   &normals[nor_count][1],
                   &normals[nor_count][2]);
            nor_count++;

            // -----------------------------------------
            // Read Faces (Triangles)
            // Supports: f v/vt/vn v/vt/vn v/vt/vn
            // -----------------------------------------
        }
        else if (strncmp(line, "f ", 2) == 0)
        {

            char *tok;
            char *ctx;
            char *tokens[4];
            int count = 0;

            tok = strtok_r(line + 2, " \t\r\n", &ctx);
            while (tok && count < 4)
            {
                tokens[count++] = tok;
                tok = strtok_r(NULL, " \t\r\n", &ctx);
            }

            if (count < 3)
                continue;

            int out_idx[6]; // triangulated indices
            int tris;

            // TRI
            if (count == 3)
            {
                tris = 3;
                out_idx[0] = 0;
                out_idx[1] = 1;
                out_idx[2] = 2;
            }
            // QUAD → split into two tris (0,1,2) and (0,2,3)
            else
            {
                tris = 6;
                int tmp[6] = {0, 1, 2, 0, 2, 3};
                for (int i = 0; i < 6; i++)
                    out_idx[i] = tmp[i];
            }

            // Allocate additional space
            vertex_buffer = realloc(vertex_buffer,
                                    sizeof(float) * (vertex_count + tris) * 6);

            if (!vertex_buffer)
            {
                fprintf(stderr, "OOM\n");
                exit(1);
            }

            for (int i = 0; i < tris; i++)
            {
                const char *t = tokens[out_idx[i]];

                int pi = parse_pos_index(t) - 1;
                int ni = parse_normal_index(t) - 1;

                float *dst = &vertex_buffer[(vertex_count + i) * 6];

                // position
                dst[0] = positions[pi][0];
                dst[1] = positions[pi][1];
                dst[2] = positions[pi][2];

                // normal
                if (ni >= 0)
                {
                    dst[3] = normals[ni][0];
                    dst[4] = normals[ni][1];
                    dst[5] = normals[ni][2];
                }
                else
                {
                    dst[3] = dst[4] = dst[5] = 0.0f; // no normal present
                }
            }

            vertex_count += tris;
        }
    }
    fclose(f);

    return (VertexData){
        .data = vertex_buffer,
        .size = vertex_count * 6,
        .vertex_size = 6};
}