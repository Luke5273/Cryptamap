#pragma once
#include "Model.hpp"
#include "View.hpp"
//#include "Shader.hpp"
#include <memory>

class Model;
class View;
class Shader;
class Overlay;

class Map
{
public:
    Map(const char* vertPath, const char* fragPath);
    ~Map();
    Map(const Map& obj) = delete;
    static Map* getInstance(const char* vertPath, const char* fragPath);
    static Map* m_self;

    uint32_t render(float widgetAspectRatio);

    Model* model; 
    View* view;
    Overlay* overlay = nullptr;

    static float verts[12];
    static uint32_t indices[6];

    uint32_t VBO;
    uint32_t VAO;
    uint32_t EBO;

    uint32_t FBO;
    uint32_t outTex;

    std::unique_ptr<Shader> shader = nullptr;
};