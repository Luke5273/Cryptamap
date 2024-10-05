

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

//struct InputTextCallback_UserData
//{
//    std::string* Str;
//    ImGuiInputTextCallback  ChainCallback;
//    void* ChainCallbackUserData;
//};
//
//int InputTextCallback(ImGuiInputTextCallbackData* data)
//{
//    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
//    if(data->EventFlag == ImGuiInputTextFlags_CallbackResize)
//    {
//        // Resize string callback
//        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
//        std::string* str = user_data->Str;
//        IM_ASSERT(data->Buf == str->c_str());
//        str->resize(data->BufTextLen);
//        data->Buf = (char*)str->c_str();
//    }
//    else if(user_data->ChainCallback)
//    {
//        // Forward to user callback, if any
//        data->UserData = user_data->ChainCallbackUserData;
//        return user_data->ChainCallback(data);
//    }
//    return 0;
//}