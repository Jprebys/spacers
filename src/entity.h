#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>

struct Vec3f
{
    float x, y, z;
};

struct Triangle
{
    Vec3f v0, v1, v2;
};

struct Mesh
{
    std::vector<Triangle> triangles;
};

Mesh make_cube();


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