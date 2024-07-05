#pragma once

class Model
{
public:
	Model() {};
	~Model();
	Model(const Model& obj) = delete;
	static Model* getInstance();
	int count = 0;

private: 
	static Model* m_self;
};

