#pragma once
#include "Panel.h"
#include "UIContext.h"
#include "Scanner.h"
#include <cstdint>
#include <string>

class AdressListPanel : public Panel{

	private:
		Scanner& sc;
		UIContext& ui;
		int selectedIndex = -1;
		int selectedType = 0;
		char editInput[32];
		void renderTable();
	public:
		explicit AdressListPanel(UIContext& ui, Scanner& sc);
		void draw() override; };
