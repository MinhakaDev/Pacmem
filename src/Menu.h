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
 
		GLFWwindow* window;

		int currentPage = 0;
		int selectedIndex = -1;
		int selectedType = 0;
		int scanValue = 0;
		static const int perPage = 20;
		char editInput[32];
        void style();

	public:
	Menu();
	~Menu();
	bool update();
};
