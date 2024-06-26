#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glm/glm.hpp>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

float scale = 1;
glm::vec2 translate = glm::vec2(0,0);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
    {
        translate -= glm::vec2(0, yoffset / 12);
        return;
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    {
        translate += glm::vec2(yoffset / 12, 0);
        return;
    }

    float prospect = scale + yoffset/12;
    scale = prospect >= 0 ? prospect : scale;
}


// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
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
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    int32_t wHeight, wWidth; //window height and width
    glfwGetWindowSize(window, &wWidth, &wHeight);

    int32_t qHeight, qWidth; //quad height and width
    qHeight = 480;
    qWidth = 640;

    float kx, ky;
    kx = (float)qWidth / wWidth;
    ky = (float)qHeight / qWidth;

    float verts[] = {
        // positions               // colors           
        0.5f*kx,  0.5f*ky, 0.0f,   1.0f, 0.0f, 0.0f,      // top right
        0.5f*kx, -0.5f*ky, 0.0f,   0.0f, 1.0f, 0.0f,      // bottom right
       -0.5f*kx, -0.5f*ky, 0.0f,   0.0f, 0.0f, 1.0f,      // bottom left
       -0.5f*kx,  0.5f*ky, 0.0f,   1.0f, 1.0f, 0.0f      // top left 
    };
    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    uint32_t VBO;
    uint32_t VAO;
    uint32_t EBO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // aColor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //compile shaders
    int success;
    char infoLog[512];

    const char* vertexSource = "#version 430 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "layout (location = 1) in vec3 aColor;\n"
                                "out vec3 col;\n"
                                "\n"
                                "uniform float scale;\n"
                                "uniform vec2 translate;\n"
                                "\n"
                                "void main() \n"
                                "{\n"
                                "    vec2 pre = aPos.xy;\n"
                                "    mat2 transform = mat2(\n"
                                "        scale, 0,\n"
                                "        0,     scale\n"
                                "    );\n"
                                "    vec2 post = transform * pre;\n"
                                "    gl_Position = vec4(post.xy + translate, aPos.z, 1.0);\n" 
                                "    col = aColor;\n" 
                                "}";
    const char* fragmentSource = "#version 430 core\n"
                                  "out vec4 FragColor;\n"
                                  "in vec3 col;\n"
                                  "\n"
                                  "void main()\n"
                                  "{\n"
                                  "    FragColor = vec4(col, 1.);\n"
                                  "}";

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) 
    {
        glGetShaderInfoLog(vertex, sizeof(infoLog), NULL, infoLog);
        std::cout << infoLog << std::endl;
    }

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, sizeof(infoLog), NULL, infoLog);
        std::cout << infoLog << std::endl;
    }

    GLuint shader = glCreateProgram();
    glAttachShader(shader, vertex);
    glAttachShader(shader, fragment);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(success)
    {
        glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::cout << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    glfwSetScrollCallback(window, scroll_callback);

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
          
        ImGui::SliderFloat("scale", &scale, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat2("pos", &translate.x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
        

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glUniform1f(glGetUniformLocation(shader, "scale"), scale);
        glUniform2f(glGetUniformLocation(shader, "translate"), translate.x, translate.y);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}