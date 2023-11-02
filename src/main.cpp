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


int main(void)
{
    Window window{"Spacers", screen_width, screen_height};
    Renderer renderer{window.m_window};

    Mesh cube = make_cube();
    renderer.FillScreen(BLACK);

    Matrix mat_proj = create_mat_proj(screen_height, screen_width, fov, f_near, f_far);

    for (auto tri : cube.triangles) {
        Triangle projected, translated;

        translated = tri;
        translated.v0.z += 3.0f;
        translated.v1.z += 3.0f;
        translated.v2.z += 3.0f;

        VecMatMul(translated.v0, projected.v0, mat_proj);
        VecMatMul(translated.v1, projected.v1, mat_proj);
        VecMatMul(translated.v2, projected.v2, mat_proj);

        projected.v0.x += 1.0f; projected.v0.y += 1.0f;
        projected.v1.x += 1.0f; projected.v1.y += 1.0f;
        projected.v2.x += 1.0f; projected.v2.y += 1.0f;

        projected.v0.x *= 0.5f * (float)screen_width;
        projected.v0.y *= 0.5f * (float)screen_height;
        projected.v1.x *= 0.5f * (float)screen_width;
        projected.v1.y *= 0.5f * (float)screen_height;
        projected.v2.x *= 0.5f * (float)screen_width;
        projected.v2.y *= 0.5f * (float)screen_height;

        renderer.DrawTriangle(projected, GREEN);
    }
    renderer.Show();

    SDL_Delay(5000);
    return 0;
}