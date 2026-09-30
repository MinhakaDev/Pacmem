#pragma once
#include "Panel.h"
#include "Scanner.h"
#include <string>
#include <vector>
 
class ProcessPanel: public Panel
{
	private:
		Scanner& sc;
        bool showPicker{true};
        int selectedIndex;
        float width;
	public:

		explicit ProcessPanel(Scanner& sc);
		void draw() override;
        void updateProcessPanel();
};
