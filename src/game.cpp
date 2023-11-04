#include "game.h"

Game::Game(char *name, int screen_width, int screen_height, 
           float fov, float f_near, float f_far)
    : m_running(true), m_elapsed_time(0), m_screen_width(screen_width), 
      m_screen_height(screen_height), m_fov(fov), m_f_near(f_near), m_f_far(f_far),
      m_window(name, screen_width, screen_height), m_renderer(m_window.m_window), 
      m_camera_pos({0, 0, 0}), m_camera_velocity({0, 0, 0}), m_movement_speed(0.1f),
      m_entity_count(0)
{
    m_proj_matrix = create_mat_proj(screen_height, screen_width, fov, f_near, f_far);

    m_entities = new Entity[MAX_ENTITIES];
}

Game::~Game()
{
    delete[] m_entities;
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            m_running = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case 'x':
                m_running = false;
                break;
            case 'w':
                m_camera_velocity.y = m_movement_speed;
                break;
            case 'a':
                m_camera_velocity.x = m_movement_speed;
                break;
            case 's':
                m_camera_velocity.y = -m_movement_speed;
                break;
            case 'd':
                m_camera_velocity.x = -m_movement_speed;
                break;
            default:
                break;
            }
            printf("x: %f    y: %f    z: %f\n", m_camera_velocity.x, m_camera_velocity.y, m_camera_velocity.z);  
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case 'w':
                m_camera_velocity.y = 0;
                break;
            case 'a':
                m_camera_velocity.x = 0;
                break;
            case 's':
                m_camera_velocity.y = 0;
                break;
            case 'd':
                m_camera_velocity.x = 0;
                break;
            default:
                break;
            }
            printf("x: %f    y: %f    z: %f\n", m_camera_velocity.x, m_camera_velocity.y, m_camera_velocity.z);  
            break;
        default:
            break;
        }
    }
}

void Game::InitGame()
{
    Vec3f positions[] = {
        {-6, 0, 40},
        {0 , 0, 40},
        {6 , 0, 40},
        {0 , 6, 40},
        {0, -6, 40},
        {4, 4, 80},
        {-4, -4, 80},
        {4, -4, 80},
        {-4, 4, 80},
 

    };

    for (size_t i = 0; i < sizeof(positions)/sizeof(Vec3f); ++i) {
        m_entities[i] = make_cube(positions[i]);
        ++m_entity_count;
    }
}

void Game::UpdatePosition()
{
    m_camera_pos = m_camera_pos + m_camera_velocity;
}

void Game::DrawScene()
{
    m_renderer.FillScreen(BLACK);

    for (size_t i = 0; i < m_entity_count; ++i) {

        Entity cube = m_entities[i];

        for (auto tri : cube.mesh.triangles) {

            tri.RotateX(m_elapsed_time * 1);
            tri.RotateY(m_elapsed_time * 2);

            Vec3f new_pos = cube.position - m_camera_pos;

            tri.Translate(new_pos);
            tri.Project(m_proj_matrix);

            if (tri.Visible()) {
                tri.Translate({1, 1, 0});
                tri.Scale(0.5f * m_screen_width, 0.5f * m_screen_height, 0);

                m_renderer.FillTriangle(tri, tri.color);
            }
        }
    }

    m_renderer.Show();
}

void Game::Run()
{
    InitGame();
    while (m_running) {
        ProcessInput();
        UpdatePosition();
        DrawScene();
        m_elapsed_time++;
        SDL_Delay(30);
    }
}

