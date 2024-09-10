

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

float scale = 1;
glm::vec2 translate = glm::vec2(0, 0);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
    ImGuiIO& io = ImGui::GetIO();
    //io.AddMouseWheelEvent(xoffset, yoffset);

    // (2) ONLY forward mouse data to your underlying app/game.
    if(!io.WantCaptureMouse)
    {
        float fac = 12;
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        {
            fac = 60;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
        {
            translate -= glm::vec2(0, (float)yoffset / fac);
            return;
        }
        else if(glfwGetKey(window, GLFW_KEY_LEFT_ALT))
        {
            translate += glm::vec2((float)yoffset / fac, 0);
            return;
        }

        float prospect = scale + (float)yoffset / fac;
        scale = prospect >= 0 ? prospect : scale;
    }
}

int width = 1280;
int height = 720;
void window_size_callback(GLFWwindow* window, int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
}

struct InputTextCallback_UserData
{
    std::string* Str;
    ImGuiInputTextCallback  ChainCallback;
    void* ChainCallbackUserData;
};

int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if(data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    else if(user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}