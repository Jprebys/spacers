#include <cstdio>
#include <cmath>

#include <SDL2/SDL.h>

#include "game.h"
#include "window.h"
#include "entity.h"
#include "renderer.h"

int main(void)
{
    int screen_width = 800;
    int screen_height = 600;
    float fov = 130.0f;
    float f_near = 0.1f;
    float f_far = 1000.0f;

    char name[] = "Spacers";

    Game game {name, screen_width, screen_height, fov, f_far, f_near};
    game.Run();
    return 0;
}