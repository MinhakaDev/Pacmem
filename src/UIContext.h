#pragma  once
#include "./Scanner.h"
#include "imgui.h"
#include <functional>


enum class Screen
{
    MAIN,
    PROCESS
};

struct MenuContext
{
    bool process = false;
};

struct UIContext{
    int selectedType  = 0;
    int selectedIndex = -1;
    int currentPage   = 0;
    Screen screen{Screen::MAIN};
    MenuContext menu;
};

