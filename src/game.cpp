#include "game.h"

Game::Game(char *name, int screen_width, int screen_height, 
           float fov, float f_near, float f_far)
    : m_running(true), m_elapsed_time(0), m_screen_width(screen_width), 
      m_screen_height(screen_height), m_fov(fov), m_f_near(f_near), m_f_far(f_far),
      m_window(name, screen_width, screen_height), m_renderer(m_window.m_window), 
      m_camera_pos({0, 0, 0})
{
    m_proj_matrix = create_mat_proj(screen_height, screen_width, fov, f_near, f_far);
}

Game::~Game()
{
}

void Game::Run()
{
    Mesh cube = make_cube();
    while (m_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                m_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == 'x')
                    m_running = false;
                break;
            default:
                break;
            }
        }

        m_renderer.FillScreen(BLACK);

        for (auto tri : cube.triangles) {
            tri.RotateX(m_elapsed_time);
            tri.RotateY(m_elapsed_time * 2);
            tri.Translate(6, 0, 5);
            tri.Project(m_proj_matrix);

            if (tri.Visible()) {
                tri.Translate(1, 1, 0);
                tri.Scale(0.5f * m_screen_width, 0.5f * m_screen_height, 0);

                m_renderer.FillTriangle(tri, tri.color);
            }
        }

        m_renderer.Show();
        m_elapsed_time++;
        SDL_Delay(30);
    }
}

