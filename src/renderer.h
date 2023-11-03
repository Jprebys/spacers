#pragma once

#include <SDL2/SDL.h>

#include "entity.h"

struct Renderer
{
    Renderer(SDL_Window *);
    ~Renderer();

    void SetColor(uint32_t color);
    void FillScreen(uint32_t color);
    void DrawLine(Vec3f v0, Vec3f v1, uint32_t color);
    void DrawTriangle(Triangle triangle, uint32_t color);
    void FillTriangle(Triangle triangle, uint32_t color);
    void DrawMesh(Mesh mesh, uint32_t color);
    void Show();

    SDL_Renderer *m_renderer;
};