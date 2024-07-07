#pragma once
#include "Model.hpp"
#include "Layer.hpp"

#include <string>
#include <format>

Layer::Layer(const char& _name)
{
	m_model = Model::getInstance();
	isVisible = true;
	name = std::string(&_name);
	inputText = std::string("");
}

Layer::Layer()
{
	m_model = Model::getInstance();
	isVisible = true;
	name = "Layer " + std::to_string(m_model->layers.size());
}