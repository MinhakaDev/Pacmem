#pragma once
#include "Panel.h"
#include "Scanner.h"
class ProcessPanel: public Panel
{
	private:
		Scanner& sc;


	public:

		explicit ProcessPanel(Scanner& sc);
		void draw() override;
};
