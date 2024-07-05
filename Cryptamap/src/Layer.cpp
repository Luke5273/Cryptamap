#pragma once
#include "Model.hpp"
#include "Layer.hpp"

#include <string>

Layer::Layer(const char& _name)
{
	m_model = Model::getInstance();
	isVisible = true;
	name = std::string(&_name);
}

Layer::Layer()
{
	m_model = Model::getInstance();
	isVisible = true;
	name = std::to_string(m_model->count);
}