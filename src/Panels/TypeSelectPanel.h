#pragma once
#include "Panel.h"
#include "UIContext.h"
#include "Scanner.h"
class TypeSelectPanel : public Panel{
	private:
	UIContext& ui;
	Scanner& sc;
	public:
    	explicit TypeSelectPanel(UIContext& ui, Scanner& sc);
	void draw() override;
};
