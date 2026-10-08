#pragma  once
#include "./Scanner.h"
#include "ErrorReporter.h"
#include "imgui.h"
#include <cstdint>
#include <functional>


enum class Screen
{
    MAIN,
    PROCESS,
    MultiLevelPointer
};

struct MenuContext
{
    bool process = false;
};

enum class ValueType { Int32, Int64, Float, Double, UINTPTR_T };

struct AdressEntry
{
	bool frozen{false};
	std::string description{""};
	uintptr_t memoryAddr;
	int type;
	std::string value{};

	explicit AdressEntry(uintptr_t memAddr, int type):memoryAddr(memAddr), type(type)
	{}
};


struct UIContext{
    const char* types[4] = { "int32", "int64", "float", "uintptr_t" };
    int selectedType  = 0;
    int selectedIndex = -1;
    int currentPage   = 0;
    std::vector<AdressEntry> adressList;
    Screen screen{Screen::MAIN};
    MenuContext menu;
};

