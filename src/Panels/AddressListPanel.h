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
        std::string editInput;
		void renderTable();
        void renderPopUp(AdressEntry& entry);
	public:
		explicit AdressListPanel(UIContext& ui, Scanner& sc);
		void draw() override; };
