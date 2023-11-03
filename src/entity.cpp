#include <cmath>

#include "entity.h"

Mesh make_cube()
{
    Mesh cube;


    float verts[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end

	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end

	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,

	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,

	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,

	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,

	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,

	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,

	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
    };

    Vec3f normals[] = {
        {-1, 0, 0}, { 0, 0, -1},
        { 0, -1, 0}, { 0, 0, -1},
        { -1, 0,0}, { 0, -1,0},
        { 0, 0, 1}, { 1, 0, 0},
        { 1, 0, 0}, { 0, 1, 0},
        { 0, 1, 0}, { 0, 0, 1} 
    };

    uint32_t colors[] = {
        0xFF0000FF, 0xFFFFFFFF,
        0xFFFF00FF, 0xFFFFFFFF,
        0xFF0000FF, 0xFFFF00FF,
        0x00FFFFFF, 0xFF00FFFF,
        0xFF00FFFF, 0x0000FFFF,
        0x0000FFFF, 0x00FFFFFF
    };

    std::vector<std::array<int, 3>> indicies {
        {4, 0, 3}, 
        {4, 3, 6},
        {0, 1, 2}, 
        {0, 2, 3},
        {1, 5, 6}, 
        {1, 6, 2},
        {5, 4, 7}, 
        {5, 7, 6},
        {7, 3, 2}, 
        {7, 2, 6},
        {0, 5, 1}, 
        {0, 4, 5} 
    };

    for (int i = 0, j = 0; i < 12; i++, j+=9) {
        Vec3f a = {verts[j + 0], verts[j + 1], verts[j + 2]};
        Vec3f b = {verts[j + 3], verts[j + 4], verts[j + 5]};
        Vec3f c = {verts[j + 6], verts[j + 7], verts[j + 8]};
        Triangle triangle {a, b, c, normals[i], colors[i]};
        cube.triangles.push_back(triangle);
    }
    return cube;
}



Matrix create_mat_proj(int screen_height, int screen_width, float fov, float f_near, float f_far)
{
    Matrix mat_proj;

    float aspect = (float)screen_height / screen_width;
    float f_fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

    mat_proj.m[0][0] = aspect * f_fov_rad;
    mat_proj.m[1][1] = f_fov_rad;
    mat_proj.m[2][2] = f_far / (f_far - f_near);
    mat_proj.m[3][2] = (-f_far * f_near) / (f_far - f_near);
    mat_proj.m[2][3] = 1.0f;

    return mat_proj;
}

void VecMatMul(Vec3f input, Vec3f &out, Matrix mat)
{
    out.x = input.x * mat.m[0][0] + input.y * mat.m[1][0] + input.z * mat.m[2][0] + mat.m[3][0];
    out.y = input.x * mat.m[0][1] + input.y * mat.m[1][1] + input.z * mat.m[2][1] + mat.m[3][1];
    out.z = input.x * mat.m[0][2] + input.y * mat.m[1][2] + input.z * mat.m[2][2] + mat.m[3][2];
    float w = input.x * mat.m[0][3] + input.y * mat.m[1][3] + input.z * mat.m[2][3] + mat.m[3][3];

    if (w != 0.0f) {
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }
    // printf("%f %f %f    %f %f %f\n", input.x, input.y, input.z, out.x, out.y, out.z);
}

// Cube::Cube()
// {
//     vertices = {
//         { 1,-1, 1}, { 1,-1,-1}, 
//         { 1, 1,-1}, { 1, 1, 1},
//         {-1,-1, 1}, {-1,-1,-1}, 
//         {-1, 1,-1}, {-1, 1, 1} 
//     };

//     normals = {
//         { 0, 0, 1}, { 0, 0, 1},
//         { 1, 0, 0}, { 1, 0, 0},
//         { 0, 0,-1}, { 0, 0,-1},
//         {-1, 0, 0}, {-1, 0, 0},
//         { 0, 1, 0}, { 0, 1, 0},
//         { 0,-1, 0}, { 0,-1, 0} 
//     };

//     colors = {
//         0x00000000
//     };

//     triangles = {
//         {{4, 0, 3}, 0, 0}, 
//         {{4, 3, 6}, 0, 0},
//         {{0, 1, 2}, 0, 0}, 
//         {{0, 2, 3}, 0, 0},
//         {{1, 5, 6}, 0, 0}, 
//         {{1, 6, 2}, 0, 0},
//         {{5, 4, 7}, 0, 0}, 
//         {{5, 7, 6}, 0, 0},
//         {{7, 3, 2}, 0, 0}, 
//         {{7, 2, 6}, 0, 0},
//         {{0, 5, 1}, 0, 0}, 
//         {{0, 4, 5}, 0, 0} 
//     };
// }
