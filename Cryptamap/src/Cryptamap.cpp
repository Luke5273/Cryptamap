#pragma once

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glm/glm.hpp>

#include <stdio.h>
#include <iostream>
#include <cmath>

#include "Shader.hpp"
#include "Model.hpp"
#include "View.hpp"
#include "Callbacks.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

auto model = Model::getInstance();

// Main code
int main(int, char**)
{
    //std::cout << model->MapData.height << "\n";
    auto view = View::getInstance(); // sets up all the opengl imgui stuff

    view->run();

    return 0;
}