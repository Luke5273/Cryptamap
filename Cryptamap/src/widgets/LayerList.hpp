#pragma once
#include "../model.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"

namespace LayerList
{
	void draw();
}

void LayerList::draw()
{
    auto model = Model::getInstance();
    ImGui::Begin("Layers");
    if(ImGui::Button("+"))
    {
        model->addLayer();
    }ImGui::SameLine();
    if(ImGui::Button("-"))
    {
        model->delLayer();
    }

    for(auto& layer : model->layers)
    {
        ImGui::PushID(&layer);
        if(ImGui::CollapsingHeader(layer.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            if(ImGui::InputTextWithHint("##", layer.name.c_str(), &layer.inputText, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if(!layer.inputText[0])
                {
                    layer.name.assign("##");
                }
                else
                {
                    layer.name.assign(layer.inputText.c_str());
                }
            }
        }
        ImGui::PopID();
    }
    ImGui::End();
}