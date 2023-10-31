#pragma once

#include <string>

#include <SDL2/SDL.h>


namespace spacers 
{

class Window
{
public:
    Window(std::string title, int width, int height);
    ~Window();
private:
    SDL_Surface *m_surface;
    SDL_Window *m_window;
};


} // namespace spacers