#pragma  once
#include "./Scanner.h"
#include "imgui.h"
struct UIContext{
    int       selectedType  = 0;
    int       selectedIndex = -1;
    int       currentPage   = 0;
};
// TypeRegistry.h
struct TypeInfo {
    const char* name;
    void (*formatValue)(Scanner&, uintptr_t addr);    // for the display
    void (*scanExact)(Scanner&, const char* input);
    void (*rescanExact)(Scanner&, const char* input);
    void (*writeValue)(Scanner&, int index, const char* input);
};

inline const TypeInfo types[] = {
    { "int32",
      [](Scanner& s, uintptr_t a){ ImGui::Text("%d",  s.getMemoryValue<int32_t>(a)); },
      [](Scanner& s, const char* in){ s.scanExact<int32_t>(std::stoi(in)); },
      [](Scanner& s, const char* in){ s.rescanExact<int32_t>(std::stoi(in)); },
      [](Scanner& s, int i, const char* in){ s.write<int32_t>(i, std::stoi(in)); } },
      
    { "int64",
      [](Scanner& s, uintptr_t a){ ImGui::Text("%lld", s.getMemoryValue<int64_t>(a)); },
      [](Scanner& s, const char* in){ s.scanExact<int64_t>(std::stoll(in)); },
      [](Scanner& s, const char* in){ s.rescanExact<int64_t>(std::stoll(in)); },
      [](Scanner& s, int i, const char* in){ s.write<int64_t>(i, std::stoll(in)); } },
      
    { "float", /* ... */ },
};

inline constexpr int typeCount = sizeof(types) / sizeof(types[0]);
