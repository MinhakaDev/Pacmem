#pragma once

#include <iostream>
#include <print>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include "Scanner.h"
#include "UIContext.h"
#include "./Panels/TypeSelectPanel.h"
#include "./Panels/MemoryScanned.h"
#include "./Panels/MainPanel.h"
#include "./Panels/MenuPanel.h"
#include "./Panels/ProcessPanel.h"
#include "./Panels/AddressListPanel.h"
#include "./Panels/MultiLevelPointerPanel.h"

class Menu
{
	private:
		Scanner sc;
		UIContext ui;
		MemoryScanned memoryScannedPannel{ui,sc};
		MainPanel mainPanel{ui,sc};
		MenuPanel menuPanel{ui};
		ProcessPanel processPanel{sc,ui};
		AdressListPanel adressListPanel{ui,sc};
        MultiLevelPointerPanel multiLvelPointerPanel{sc,ui};
 

		GLFWwindow* window;

        void style();

	public:
	Menu();
	~Menu();
	bool update();
};
