#pragma once
#include "Panel.h"
#include "Scanner.h"
#include "UIContext.h"
#include <string>
#include <vector>
#include "UIContext.h"
 
class ProcessPanel: public Panel
{
	private:
		Scanner& sc;
        UIContext& ui;
        int selectedIndex;
        float width;
	public:

		explicit ProcessPanel(Scanner& sc, UIContext& ui);
		void draw() override;
        void updateProcessPanel();
};
