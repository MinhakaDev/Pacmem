#pragma  once
#include "./Scanner.h"
#include "imgui.h"
#include <functional>
struct UIContext{
    int       selectedType  = 0;
    int       selectedIndex = -1;
    int       currentPage   = 0;
};

