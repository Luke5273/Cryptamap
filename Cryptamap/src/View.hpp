#pragma once 
#include <glm/glm.hpp>
//#include "Map.hpp"
#include "Model.hpp"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Map;

class View
{
public:
	View() {};
	~View();
	View(const View& obj) = delete;
	static View* getInstance();

	GLFWwindow* window = nullptr;
	Model* model = Model::getInstance();

	void init();
	void renderMap();
	void clean();

	struct _transforms {
		float scale = 1;
		glm::vec2 translate = glm::vec2(0, 0);
	} transforms;

	struct _style {
		ImVec4 bgColour = ImVec4(.2f, .2f, .4f, 1.0f);
	} style;

	Map* map = nullptr;

private:
	static View* m_self;
};

