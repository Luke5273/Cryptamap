#pragma once
#include "Layer.hpp"
#include <vector>

class Model
{
public:
	Model() {};
	~Model();
	Model(const Model& obj) = delete;
	static Model* getInstance();
	int count = 0;

	std::vector<Layer> layers;
	void addLayer();
	void delLayer();
	void delLayer(int loc);

private: 
	static Model* m_self;
};

