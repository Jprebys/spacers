#pragma once

#include <cstdint>

#include "entity.h"
#include "window.h"
#include "renderer.h"


struct Game
{
    Game(char *name, int screen_width, int screen_height, float fov, float f_near, float f_far);
    ~Game();
    void Run();
    
    bool m_running;
    uint64_t m_elapsed_time;

    int m_screen_width;
    int m_screen_height;
    float m_fov;
    float m_f_near;
    float m_f_far;
    Matrix m_proj_matrix;

    Window m_window;
    Renderer m_renderer;

    Vec3f m_camera_pos;
};
