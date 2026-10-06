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
        std::string searchInput;
		void renderToolbar();
		void renderScanCombo();


	public:
		explicit MainPanel(UIContext& ui, Scanner& sc);
		void draw() override;
};
