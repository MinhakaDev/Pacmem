#pragma once
#include "MultiLevelPointer.h"
#include "Panels/Panel.h"
#include "UIContext.h"
#include "panel.h"
#include <curses.h>
#include <Scanner.h>
#include <vector>




class MultiLevelPointerPanel: public Panel
{
    private:

        Scanner& sc;
        MultiLevelPointer mpl;
        std::string scanTarget;
        int scanDepth{0};
        std::vector<PointerNode> nodes{};
        UIContext& ui;
    public:
        MultiLevelPointerPanel(Scanner& sc, UIContext& ui);
        void draw() override;
        void renderTable(int depth, std::vector<PointerNode>& pointers);
};

