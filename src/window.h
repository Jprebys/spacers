#pragma once

#include <string>

#include <SDL2/SDL.h>


struct Window
{
    Window(std::string title, int width, int height);
    ~Window();
    SDL_Surface *m_surface;
    SDL_Window *m_window;
};
