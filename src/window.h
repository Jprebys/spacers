#pragma once

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>


struct Window
{
    Window(std::string title, int width, int height);
    ~Window();

    void UpdateTitle(uint32_t frame_time);

    SDL_Surface *m_surface;
    SDL_Window *m_window;
    std::string m_title;
};
