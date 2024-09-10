#pragma once
#include "View.hpp"

// Stuff required for singleton
View* View::m_self = nullptr;

View* View::getInstance()
{
	if(m_self == nullptr)
	{
		m_self = new View;
	}
	return m_self;
}

View::~View()
{
	delete m_self;
}