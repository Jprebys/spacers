#include <stdexcept>

#include "renderer.h"


Renderer::Renderer(SDL_Window *window)
{
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!m_renderer) {
        printf("[ERROR] Could not create renderer\n");
        throw std::runtime_error("Could not create renderer");
    }
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_renderer);
}

void Renderer::SetColor(uint32_t color)
{
    uint8_t r = (color >> 24) & 0xFF;
    uint8_t g = (color >> 16) & 0xFF;
    uint8_t b = (color >>  8) & 0xFF;
    uint8_t a = (color >>  0) & 0xFF;
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void Renderer::FillScreen(uint32_t color)
{
    SetColor(color);
    SDL_RenderClear(m_renderer);
}

void Renderer::DrawLine(Vec3f v0, Vec3f v1, uint32_t color)
{
    SetColor(color);
    SDL_RenderDrawLineF(m_renderer, v0.x, v0.y, v1.x, v1.y);
}

void Renderer::DrawTriangle(Triangle triangle, uint32_t color)
{
    SetColor(color);
    DrawLine(triangle.v0, triangle.v1, color);
    DrawLine(triangle.v1, triangle.v2, color);
    DrawLine(triangle.v2, triangle.v0, color);
}

void Renderer::DrawMesh(Mesh mesh, uint32_t color)
{
    SetColor(color);
    for (auto triangle : mesh.triangles) {
        DrawTriangle(triangle, color);
    }
}

void Renderer::Show()
{
    SDL_RenderPresent(m_renderer);
}
