#pragma once
#include <string>
// #include "Model.hpp"
class Model;

/*TODO: ADD TEXTURES AND ALL THAT JAZZ*/
class Layer 
{
public:
	bool isVisible;
	std::string name;
	std::string inputText;

	Layer(const char& _name);
	Layer();

private:
	Model* m_model;
};