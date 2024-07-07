#pragma once
#include "Model.hpp"

// Stuff required for singleton
Model* Model::m_self = nullptr;

Model* Model::getInstance()
{
	if(m_self == nullptr)
	{
		m_self = new Model;
	}
	return m_self;
}

Model::~Model()
{
	delete m_self;
}

//Layer list logic
void Model::addLayer()
{
	auto newLayer = Layer();
	layers.push_back(newLayer);
}
		
void Model::delLayer()
{
	layers.pop_back();
}

void Model::delLayer(int loc)
{
	layers.erase(layers.begin() + loc);
}
