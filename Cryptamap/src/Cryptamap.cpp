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
#include "widgets/LayerList.hpp"
#include "Callbacks.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

auto model = Model::getInstance();

// Main code
int main(int, char**)
{
    auto view = View::getInstance(); // sets up all the opengl imgui stuff
    while(!glfwWindowShouldClose(view->window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        ImGui::ShowDemoWindow();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Hello, world!", NULL); //ImGuiWindowFlags_NoDecoration                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("scale", &view->transforms.scale, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat2("pos", &view->transforms.translate.x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat2("pos", &view->transforms.translate.x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        view->renderMap();

        LayerList::draw();   

        // Rendering
        ImGui::Render();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        int display_w, display_h;
        glfwGetFramebufferSize(view->window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImVec4 clear_color = view->style.bgColour;
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        

        glfwSwapBuffers(view->window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(view->window);
    glfwTerminate();

    return 0;
}