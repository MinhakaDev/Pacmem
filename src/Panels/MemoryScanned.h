#pragma once
#include "Panel.h"
#include "UIContext.h"
#include "Scanner.h"
class MemoryScanned : public Panel{

	private:
		Scanner& sc;
		UIContext& ui;
		int selectedIndex = -1;
		int selectedType = 0;
        std::string editInput;
	public:
		explicit MemoryScanned(UIContext& ui, Scanner& sc);
		void draw() override;
};
