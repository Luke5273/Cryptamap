#pragma once
#include "Model.hpp"
#include "View.hpp"
//#include "Shader.hpp"
#include <memory>

class Model;
class View;
class Shader;

class Overlay
{
public:
    Overlay(const char* vertPath, const char* fragPath);
    ~Overlay();
    Overlay(const Overlay& obj) = delete;
    static Overlay* getInstance(const char* vertPath, const char* fragPath);
    static Overlay* m_self;

    void render(uint32_t FBO, float widgetAspectRatio);

    Model* model;
    View* view;
    static float verts[12];
    static uint32_t indices[6];

    uint32_t VBO;
    uint32_t VAO;
    uint32_t EBO;

    uint32_t m_FBO;
    uint32_t outTex;

    std::unique_ptr<Shader> shader = nullptr;
};