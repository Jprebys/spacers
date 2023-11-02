#include <cstdio>

#include <SDL2/SDL.h>

#include "window.h"
#include "entity.h"
#include "renderer.h"

#define RED   0xFF0000FF
#define GREEN 0x00FF00FF
#define BLUE  0x0000FFFF
#define WHITE 0xFFFFFFFF
#define BLACK 0x000000FF


int main(void)
{
    printf("Hello, world!\n");

    Window window{"Spacers", 800, 600};
    Mesh cube = make_cube();

    Renderer renderer{window.m_window};

    renderer.FillScreen(BLACK);
    renderer.DrawMesh(cube, GREEN);
    renderer.Show();

    SDL_Delay(5000);
    return 0;
}