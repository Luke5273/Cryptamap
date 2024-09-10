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


	struct _MapData {
		int32_t width = 30;
		int32_t height = 20;
		int32_t dpi = 70;
		int32_t pixWidth = width*dpi;
		int32_t pixHeight = height*dpi;
	} MapData;

private: 
	static Model* m_self;
};

