#pragma once
#include "View.hpp"
#include "Model.hpp"
#include "Map.hpp"
#include "Overlay.hpp"
#include "Callbacks.hpp"
#include "widgets/LayerList.hpp"

#include "iostream"

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glm/glm.hpp>

// Stuff required for singleton
View* View::m_self = nullptr;

View* View::getInstance()
{
	if(m_self == nullptr)
	{
		m_self = new View;
        m_self->init();
	}
	return m_self;
}

View::~View()
{
    clean();
	delete m_self;
}

void View::init()
{
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit())
        exit(1);

    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if(window == nullptr)
        exit(1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        exit(1);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(.2f, .2f, .2f, 1.00f);

    //auto model = Model::getInstance();

    map = Map::getInstance("./shaders/test.vert", "./shaders/test.frag");

    glBindFramebuffer(GL_FRAMEBUFFER, map->FBO);
    glClearColor(1.f, 0.f, 0.f, 1.f); //red, to see if rendering doesnt work
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void View::run()
{
    while(!glfwWindowShouldClose(window))
    {
        update();
    }
}

void View::update()
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

        ImGui::SliderFloat("scale",&transforms.scale, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat2("pos", &transforms.translate.x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat2("pos", &transforms.translate.x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    renderMap();

    LayerList::draw();

    // Rendering
    ImGui::Render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImVec4 clear_color = style.bgColour;
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }


    glfwSwapBuffers(window);
}

void View::renderMap()
{
    ImGui::Begin("Image", NULL, ImGuiWindowFlags_NoDecoration);

    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();
    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;
    ImVec2 widgetSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if(io.MouseWheel && ImGui::IsWindowHovered())
    {
        bool shouldScale = true;

        float fac = 12;
        if(ImGui::IsKeyDown(ImGuiKey_LeftShift))
        {
            fac = 60;
        }
        if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            transforms.translate -= glm::vec2(0, (float)io.MouseWheel / fac);
            shouldScale = false;
        }
        else if(ImGui::IsKeyDown(ImGuiKey_LeftAlt))
        {
            transforms.translate += glm::vec2((float)io.MouseWheel / fac, 0);
            shouldScale = false;
        }

        if(shouldScale)
        {
            float prospect = transforms.scale + (float)io.MouseWheel / fac;
            transforms.scale = prospect >= 0 ? prospect : transforms.scale;
        }
    }

    map->render(widgetSize.x/widgetSize.y);

    ImGui::Image((void*)(intptr_t)map->outTex, widgetSize);

    ImGui::End();
}

void View::clean()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}