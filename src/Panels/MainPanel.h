#pragma once
#include "Panel.h"
#include "UIContext.h"
#include "Scanner.h"
class MainPanel : public Panel{

	private:
		Scanner& sc;
		UIContext& ui;
		int selectedIndex = -1;
		int selectedType = 0;
		char editInput[32];
		char searchInput[32];
		void renderToolbar();
		void renderScanCombo();


	public:
		explicit MainPanel(UIContext& ui, Scanner& sc);
		void draw() override;
};
