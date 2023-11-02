#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>

struct Vec3f
{
    float x, y, z;

    void operator+(float n) {x += n; y += n; z += n;}
};

struct Triangle
{
    Vec3f v0, v1, v2;
};

struct Mesh
{
    std::vector<Triangle> triangles;
};

struct Matrix
{
    float m[4][4] = {0};
};

Mesh make_cube();
Matrix create_mat_proj(int screen_height, int screen_width, float fov, float f_near, float f_far);
void VecMatMul(Vec3f input, Vec3f &out, Matrix mat);


// struct Triangle 
// {
//     int vertexIndices[3];
//     int normalIndex;
//     int colorIndex;
// };

// struct Geometry {
//     std::vector<Vec3f> vertices;
//     std::vector<Vec3f> normals;
//     std::vector<uint32_t> colors;
//     std::vector<Triangle> triangles;
// };

// struct Cube : public Geometry {
//     Cube();
// };