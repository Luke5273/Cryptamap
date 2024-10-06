#pragma once
#include "Canvas.hpp"
#include "Shader.hpp"
#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glm/glm.hpp>

//Canvas* Canvas::m_self = nullptr;
//
//Canvas* Canvas::getInstance()
//{
//    if(m_self == nullptr)
//    {
//        m_self = new Canvas;
//    }
//    return m_self;
//}

float Canvas::verts[12] = {
    // positions         
    1.0f,  1.0f, 0.0f,  // top right
    1.0f, -1.0f, 0.0f,  // bottom right
   -1.0f, -1.0f, 0.0f,  // bottom left
   -1.0f,  1.0f, 0.0f,  // top left 
};

uint32_t Canvas::indices[6] = {
    0, 1, 2,
    0, 2, 3
};

Canvas::Canvas()
{
    model = Model::getInstance();
    view = View::getInstance();
    shader = std::make_unique<Shader>("./shaders/test.vert", "./shaders/test.frag");
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &outTex);
    glBindTexture(GL_TEXTURE_2D, outTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, model->MapData.pixWidth, model->MapData.pixHeight, NULL, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outTex, 0);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;
}

//Canvas::~Canvas()
//{
//    delete m_self;
//}

uint32_t Canvas::render(float widgetAspectRatio)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glViewport(0, 0, model->MapData.pixWidth, model->MapData.pixHeight);
    glClearColor(view->style.bgColour.x * view->style.bgColour.w,
        view->style.bgColour.y * view->style.bgColour.w,
        view->style.bgColour.z * view->style.bgColour.w,
        view->style.bgColour.w
    );
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();
    glUniform1f(glGetUniformLocation(shader->ID, "scale"), view->transforms.scale);
    glUniform2f(glGetUniformLocation(shader->ID, "translate"), view->transforms.translate.x, view->transforms.translate.y);
    glUniform1f(glGetUniformLocation(shader->ID, "aspectRatio"), widgetAspectRatio * (float)model->MapData.height / model->MapData.width);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return outTex;
}