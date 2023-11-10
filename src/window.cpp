
#include <cstdlib>
#include <cstdio>
#include <cinttypes>

#include <SDL2/SDL.h>

#include "window.h"


Window::Window(std::string title, int height, int width)
  : m_title(title)
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

void Window::UpdateTitle(uint32_t frame_time)
{
    const size_t NEW_TITLE_LEN = 128;
    char new_title[NEW_TITLE_LEN];
    snprintf(new_title, NEW_TITLE_LEN, "%s (%" PRIu32 " ms)", m_title.c_str(), frame_time);
    SDL_SetWindowTitle(m_window, new_title);
}

Window::~Window()
{
    SDL_DestroyWindow(m_window);
}
