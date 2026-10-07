#pragma once
#include "Panel.h"
#include "UIContext.h"
#include "Scanner.h"

#include "../MultiLevelPointer.h"
class MainPanel : public Panel{

	private:
		Scanner& sc;
		UIContext& ui;
		int selectedIndex = -1;
		int selectedType = 0;
        std::string searchInput;
		void renderToolbar();
		void renderScanCombo();


    // resolver
    std::string chainPath;
    std::string chainOffsets;
    std::string resolveStatus;

    // pointer scan
    std::string scanTarget;
    int scanDepth = 1;
    std::string scanStatus;
    PointerNode lastScan;

	public:
		explicit MainPanel(UIContext& ui, Scanner& sc);
		void draw() override;

        MultiLevelPointer mlp;
        void renderTest();

};
