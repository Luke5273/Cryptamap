#pragma once
#include "Model.hpp"
#include <string>

/*TODO: ADD TEXTURES AND ALL THAT JAZZ*/
class Layer 
{
public:
	bool isVisible;
	std::string name;

	Layer(const char& _name);
	Layer();

private:
	Model* m_model;
};