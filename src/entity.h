#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>

struct Matrix
{
    float m[4][4] = {0};
};

struct Vec3f
{
    float x, y, z;

    Vec3f operator-();
    Vec3f operator-(Vec3f other);
    Vec3f operator+(Vec3f other);
    Vec3f &operator=(float val);
};

struct Triangle
{
    Vec3f v0, v1, v2;

    Vec3f norm;

    uint32_t color;

    void Translate(Vec3f vec);
    void Scale(float x, float y, float z);
    void RotateY(int degrees);
    void RotateX(int degrees);
    void Project(Matrix proj_matrix);

    bool Visible();
};

struct Mesh
{
    std::vector<Triangle> triangles;
};

struct Entity
{
    Mesh mesh;
    Vec3f position;
    bool alive;
};

Entity make_cube(Vec3f position);
Matrix create_mat_proj(int screen_height, int screen_width, float fov, float f_near, float f_far);
void VecMatMul(Vec3f input, Vec3f &out, Matrix mat);
float VecDot(Vec3f a, Vec3f b);

Vec3f rotate_x(Vec3f input, int degrees);
Vec3f rotate_y(Vec3f input, int degrees);


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