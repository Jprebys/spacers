#include <cmath>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "entity.h"


Entity create_entity_from_file(char *filename, uint32_t color, Vec3f position)
{
    Entity result;
    std::vector<Vec3f> verts;
    std::vector<Vec3f> norms;

    float x, y, z;
    int v0, v1, v2;
    int t0, t1, t2;
    int n0, n1, n2;
    int rc;
    Vec3f temp;

    std::ifstream obj_file(filename);
    for (std::string line; getline(obj_file, line); ) {
        if (line.length() > 1) {
            if (line[0] == 'v' && line[1] == ' ') {
                // str >> code >> x >> y >> z;
                rc = sscanf(line.c_str(), "v %f %f %f ", &x, &y, &z);
                if (rc != 3) {
                    printf("Bad line: %s\n", line.c_str());
                    throw std::runtime_error("Error reading line in obj file");
                }
                verts.push_back({x, y, z});
            } else if (line[0] == 'v' && line[1] == 'n') {
                // str >> code >> x >> y >> z;
                rc = sscanf(line.c_str(), "vn %f %f %f ", &x, &y, &z);
                if (rc != 3) {
                    printf("Bad line: %s\n", line.c_str());
                    throw std::runtime_error("Error reading line in obj file");
                }
                norms.push_back({x, y, z});

            } else if (line[0] == 'f' && line[1] == ' ') {
                if (norms.size() > 0) {
                    // Our file has normal info
                    rc = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d ", &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2);
                    if (rc != 9) {
                        printf("Bad line: %s\n", line.c_str());
                        throw std::runtime_error("Error reading line in obj file");
                    }
                    Vec3f vec0 = verts[v0 - 1];
                    Vec3f vec1 = verts[v1 - 1];
                    Vec3f vec2 = verts[v2 - 1];
                    Vec3f norm = norms[n0 - 1];
                    Triangle tri {vec0, vec1, vec2, norm, color};
                    result.mesh.triangles.push_back(tri);     
                } else {
                    // Our file does not have normal info, need to calculate it
                    rc = sscanf(line.c_str(), "f %d %d %d ", &v0, &v1, &v2);
                    if (rc != 3) {
                        printf("Bad line: %s\n", line.c_str());
                        throw std::runtime_error("Error reading line in obj file");
                    }
                    Vec3f vec0 = verts[v0 - 1];
                    Vec3f vec1 = verts[v1 - 1];
                    Vec3f vec2 = verts[v2 - 1];
                    Vec3f norm = VecCross(vec1 - vec0, vec2 - vec0);
                    Triangle tri {vec0, vec1, vec2, norm, color};
                    result.mesh.triangles.push_back(tri);
                }

            }
        }
    }

    result.alive = true;
    result.position = position;

    return result;
}


Matrix create_mat_proj(int screen_height, int screen_width, float fov, float f_near, float f_far)
{
    Matrix mat_proj;

    float aspect = (float)screen_height / screen_width;
    float f_fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

    mat_proj.m[0][0] = aspect * f_fov_rad;
    mat_proj.m[1][1] = f_fov_rad;
    mat_proj.m[2][2] = f_far / (f_far - f_near);
    mat_proj.m[2][3] = (-f_far * f_near) / (f_far - f_near);
    mat_proj.m[3][2] = 1.0f;

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

float VecDot(Vec3f a, Vec3f b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3f VecCross(Vec3f a, Vec3f b) {
    float x = a.y * b.z - a.z * b.y;
    float y = a.z * b.y - a.x * b.z;
    float z = a.x * b.y - a.y * b.x;
    return {x, y, z};
}

Vec3f Vec3f::operator-()
{
    return {-x, -y, -z};
}

Vec3f Vec3f::operator-(Vec3f other)
{
    return {x + other.x, y + other.y, z + other.z};
}

Vec3f Vec3f::operator+(Vec3f other)
{
    return {x - other.x, y - other.y, z - other.z};
}

Vec3f &Vec3f::operator=(float val)
{
    x = val; y = val; z = val;
    return *this;
}

Vec3f Vec3f::operator/(float val)
{
    return {x / val, y / val, z / val};
}

float Vec3f::Length()
{
    return sqrtf(x*x + y*y + z*z);
}


bool Triangle::Visible()
{
    float determinant = v0.x * (v1.y - v2.y) +
                        v1.x * (v2.y - v0.y) +
                        v2.x * (v0.y - v1.y);
    return determinant < 0;
}

void Triangle::Translate(Vec3f update)
{
    v0.x += update.x; v0.y += update.y; v0.z += update.z;
    v1.x += update.x; v1.y += update.y; v1.z += update.z;
    v2.x += update.x; v2.y += update.y; v2.z += update.z;
}

Vec3f rotate_x(Vec3f input, int degrees) 
{
    float rads = (float)degrees * 3.14159f / 180.0f;
    Matrix mat;

    mat.m[0][0] = 1.0f;
    mat.m[1][1] = cosf(rads);
    mat.m[1][2] = sinf(rads);
    mat.m[2][1] = -sinf(rads);
    mat.m[2][2] = cosf(rads);
    mat.m[3][3] = 1.0f;

    Vec3f result;
    VecMatMul(input, result, mat);
    return result;
}

Vec3f rotate_y(Vec3f input, int degrees) 
{
    float rads = (float)degrees * 3.14159f / 180.0f;
    Matrix mat;

    mat.m[0][0] = cosf(rads);
    mat.m[0][2] = sinf(rads);
    mat.m[1][1] = 1.0f;
    mat.m[2][0] = -sinf(rads);
    mat.m[2][2] = cosf(rads);
    mat.m[3][3] = 1.0f;

    Vec3f result;
    VecMatMul(input, result, mat);
    return result;
}

void Triangle::RotateX(int degrees)
{
    v0 = rotate_x(v0, degrees);
    v1 = rotate_x(v1, degrees);
    v2 = rotate_x(v2, degrees);
    norm = rotate_x(norm, degrees); 
}

void Triangle::RotateY(int degrees)
{
    v0 = rotate_y(v0, degrees);
    v1 = rotate_y(v1, degrees);
    v2 = rotate_y(v2, degrees);
    norm = rotate_y(norm, degrees);    
}

void Triangle::Scale(float x, float y, float z)
{
    v0.x *= x; v1.x *= x; v2.x *= x;
    v0.y *= y; v1.y *= y; v2.y *= y;
    v0.z *= z; v1.z *= z; v2.z *= z;
}

void Triangle::Project(Matrix proj_matrix)
{
    VecMatMul(v0, v0, proj_matrix);
    VecMatMul(v1, v1, proj_matrix);
    VecMatMul(v2, v2, proj_matrix);
    // VecMatMul(norm, norm, proj_matrix); 
}

Entity make_cube(Vec3f position)
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

    // uint32_t colors[] = {
    //     0xFF0000FF, 0xFFFFFFFF,
    //     0xFFFF00FF, 0xFFFFFFFF,
    //     0xFF0000FF, 0xFFFF00FF,
    //     0x00FFFFFF, 0xFF00FFFF,
    //     0xFF00FFFF, 0x0000FFFF,
    //     0x0000FFFF, 0x00FFFFFF
    // };

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
        // Triangle triangle {a, b, c, normals[i], colors[i]};
        Triangle triangle {a, b, c, normals[i], 0xFF00FFFF};

        cube.triangles.push_back(triangle);
    }

    bool alive = true;
    return {cube, position, alive};
}

