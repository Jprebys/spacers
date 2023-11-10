#include <algorithm>

#include "game.h"

#define TRIANGLE_VEC_SIZE 1024 * 1024
#define MIN_BRIGHTNESS .1f
#define FPS 60
#define FRAME_TIME 1000 / FPS


Game::Game(char *name, int screen_width, int screen_height, 
           float fov, float f_near, float f_far)
    : m_running(true), m_elapsed_time(0), m_screen_width(screen_width), 
      m_screen_height(screen_height), m_fov(fov), m_f_near(f_near), m_f_far(f_far),
      m_window(name, screen_width, screen_height), m_renderer(m_window.m_window), 
      m_camera_pos({0, 0, 0}), m_camera_look({0, 0, 1}), m_camera_velocity({0, 0, 0}),
      m_movement_speed(0.1f), m_entity_count(0)
{
    m_proj_matrix = create_mat_proj(screen_height, screen_width, fov, f_near, f_far);

    m_entities = new Entity[MAX_ENTITIES];

    m_triangles.reserve(TRIANGLE_VEC_SIZE);
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
            // printf("x: %f    y: %f    z: %f\n", m_camera_velocity.x, m_camera_velocity.y, m_camera_velocity.z);  
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
            // printf("x: %f    y: %f    z: %f\n", m_camera_velocity.x, m_camera_velocity.y, m_camera_velocity.z);  
            break;
        default:
            break;
        }
    }
}

void Game::InitGame()
{
    Vec3f positions[] = {
        {0 , 0, 5},
    };
    char teapot_file[] = "assets/teapot2.obj";

    for (size_t i = 0; i < sizeof(positions)/sizeof(Vec3f); ++i) {
        m_entities[i] = create_entity_from_file(teapot_file, 0xBBBBBBFF, positions[i]);

        ++m_entity_count;
    }
}

void Game::UpdatePosition()
{
    m_camera_pos = m_camera_pos + m_camera_velocity;
}

void Game::DrawScene()
{
    uint32_t start = SDL_GetTicks();

    m_renderer.FillScreen(BLACK);

    Vec3f light {0, 0, -1};
    light.Normalize();

    Vec3f up_direction {0, 1, 0};
    Vec3f target = m_camera_pos + m_camera_look;

    Matrix camera_matrix;
    camera_matrix.PointAt(m_camera_pos, target, up_direction);
    camera_matrix.Invert();


    for (size_t i = 0; i < m_entity_count; ++i) {

        Entity entity = m_entities[i];

        for (auto tri : entity.mesh.triangles) {

            tri.RotateX(m_elapsed_time * 1);
            tri.RotateY(m_elapsed_time * 2);
            tri.RotateZ(m_elapsed_time * .5);

            // Vec3f new_pos = entity.position - m_camera_pos;

            // tri.Translate(new_pos);
            tri.Translate(entity.position);
            Vec3f norm = VecCross(tri.v1 - tri.v0, tri.v2 - tri.v0);
            norm.Normalize();

            if (VecDot(norm, tri.v0 - m_camera_pos) < 0) {

                float dp = VecDot(norm, light);
                dp = std::max(MIN_BRIGHTNESS, dp);

                VecMatMul(tri.v0, tri.v0, camera_matrix);
                VecMatMul(tri.v1, tri.v1, camera_matrix);
                VecMatMul(tri.v2, tri.v2, camera_matrix);

                tri.Project(m_proj_matrix);

                if (tri.OnScreen()) {

                    tri.Translate({1, 1, 0});
                    tri.Scale(0.5f * m_screen_width, 0.5f * m_screen_height, 1);

                    m_triangles.emplace_back(tri, dp);
                }
            }
        }
    }

    std::sort(m_triangles.begin(), m_triangles.end(), &TriangleDepthCmp);

    for (auto [tri, brightness] : m_triangles) {
        m_renderer.FillTriangle(tri, tri.color, brightness);
        // m_renderer.DrawTriangle(tri, WHITE);
    }

    m_renderer.Show();
    m_triangles.clear();

    uint32_t frame_time_ms = SDL_GetTicks() - start;
    m_window.UpdateTitle(frame_time_ms);
    if (frame_time_ms < FRAME_TIME)
        SDL_Delay(FRAME_TIME - frame_time_ms);
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

