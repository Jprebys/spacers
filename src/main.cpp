#include <cstdio>
#include <cmath>

#include <SDL2/SDL.h>

#include "window.h"
#include "entity.h"
#include "renderer.h"

#define RED   0xFF0000FF
#define GREEN 0x00FF00FF
#define BLUE  0x0000FFFF
#define WHITE 0xFFFFFFFF
#define BLACK 0x000000FF

int screen_width = 800;
int screen_height = 600;
float fov = 90.0f;
float f_near = 0.1f;
float f_far = 1000.0f;

float VecDot(Vec3f a, Vec3f b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool TriVisible(Triangle tri)
{
    float determinant = tri.v0.x * (tri.v1.y - tri.v2.y) +
                        tri.v1.x * (tri.v2.y - tri.v0.y) +
                        tri.v2.x * (tri.v0.y - tri.v1.y);
    return determinant < 0;
}

Vec3f RotateX(Vec3f input, int degrees) {
    float rads = (float)degrees * 3.14159f / 180.0f;
    Matrix mat;

    mat.m[0][0] = 1.0f;
    mat.m[1][1] = cos(rads);
    mat.m[1][2] = -sin(rads);
    mat.m[2][1] = sin(rads);
    mat.m[2][2] = cos(rads);

    Vec3f result;
    VecMatMul(input, result, mat);
    return result;
}

Vec3f RotateY(Vec3f input, int degrees) {
    float rads = (float)degrees * 3.14159f / 180.0f;
    Matrix mat;

    mat.m[0][0] = cos(rads);
    mat.m[2][0] = sin(rads);
    mat.m[1][1] = 1.0f;
    mat.m[2][0] = -sin(rads);
    mat.m[2][2] = cos(rads);

    Vec3f result;
    VecMatMul(input, result, mat);
    return result;
}


int main(void)
{
    Window window{"Spacers", screen_width, screen_height};
    Renderer renderer{window.m_window};


    Mesh cube = make_cube();

    Matrix mat_proj = create_mat_proj(screen_height, screen_width, fov, f_near, f_far);
    bool running = true;
    uint64_t elapsed_time = 0;
    SDL_Event event;
    while (running) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == 'x')
                    running = false;
                break;
            default:
                break;
            }
        }

        renderer.FillScreen(BLACK);

        for (auto tri : cube.triangles) {
            Triangle projected, translated, rotated;

            rotated = tri;
            rotated.v0 = RotateX(rotated.v0, elapsed_time);
            rotated.v1 = RotateX(rotated.v1, elapsed_time);
            rotated.v2 = RotateX(rotated.v2, elapsed_time);
            rotated.norm = RotateX(rotated.norm, elapsed_time); 

            rotated.v0 = RotateY(rotated.v0, elapsed_time * 2);
            rotated.v1 = RotateY(rotated.v1, elapsed_time * 2);
            rotated.v2 = RotateY(rotated.v2, elapsed_time * 2);
            rotated.norm = RotateY(rotated.norm, elapsed_time * 2); 

            translated = rotated;
            translated.v0.z += 3.0f;
            translated.v1.z += 3.0f;
            translated.v2.z += 3.0f;

            VecMatMul(translated.v0, projected.v0, mat_proj);
            VecMatMul(translated.v1, projected.v1, mat_proj);
            VecMatMul(translated.v2, projected.v2, mat_proj);
            VecMatMul(translated.norm, projected.norm, mat_proj);

            if (TriVisible(projected)) {
            // if (true) {
                projected.v0.x += 1.0f; projected.v0.y += 1.0f;
                projected.v1.x += 1.0f; projected.v1.y += 1.0f;
                projected.v2.x += 1.0f; projected.v2.y += 1.0f;

                projected.v0.x *= 0.5f * (float)screen_width;
                projected.v0.y *= 0.5f * (float)screen_height;
                projected.v1.x *= 0.5f * (float)screen_width;
                projected.v1.y *= 0.5f * (float)screen_height;
                projected.v2.x *= 0.5f * (float)screen_width;
                projected.v2.y *= 0.5f * (float)screen_height;

                renderer.FillTriangle(projected, tri.color);
            }
        }
        renderer.Show();
        elapsed_time++;
        SDL_Delay(30);
    }

    return 0;
}