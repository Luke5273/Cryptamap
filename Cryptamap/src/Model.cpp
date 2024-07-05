#pragma once
#include "Model.hpp"

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

Model* Model::m_self = nullptr;