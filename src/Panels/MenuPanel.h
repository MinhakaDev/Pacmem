#pragma once
#include "Panels/Panel.h"
#include "UIContext.h"
#include <cstdint>

class MenuPanel: public Panel
{
	private:
        UIContext& ui;

	public:
        MenuPanel(UIContext& ui);
		void draw() override;
};
