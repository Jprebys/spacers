
#include <cstdlib>
#include <cstdio>

#include <SDL2/SDL.h>

#include "window.h"


Window::Window(std::string title, int height, int width)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("[ERROR] Failed to initialize SDL\n");
        exit(EXIT_FAILURE);
    } 
    m_window = SDL_CreateWindow(title.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                height, width,
                                0);
    if (!m_window) {
        printf("[ERROR] Failed to create window\n");
        exit(EXIT_FAILURE); 
    }

    m_surface = SDL_GetWindowSurface(m_window);
    if (!m_surface) {
        printf("[ERROR] Failed to get the surface from the window\n");
        exit(EXIT_FAILURE); 
    }

    SDL_UpdateWindowSurface(m_window);
}

Window::~Window()
{
    SDL_DestroyWindow(m_window);
}
