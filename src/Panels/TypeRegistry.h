#pragma once
#include "Scanner.h"
#include <cstdint>
#include <string>
#include <vector>
#include "imgui.h"
struct TypeInfo{
	const char* name;
	void(*scanExact)(Scanner&,const char* input);
	void(*rescanExact)(Scanner&, const char* input);
	void(*writeMemory)(Scanner&, int index, const char* input);
	void(*getMemoryValue)(Scanner&, uintptr_t memoryAddr);
	void(*renderMemoryValue)(Scanner&, uintptr_t memoryAddr);
};

inline const TypeInfo types[] =
{
	{
		"int32",
		[](Scanner& sc, const char* input){sc.scanExact<int32_t>(std::stoi(input));},
		[](Scanner& sc, const char* input){sc.rescanExact<int32_t>(std::stoi(input));},
		[](Scanner& sc, int index, const char* input){sc.write<int32_t>(index, std::stoi(input));},
		[](Scanner& sc, uintptr_t memoryAddr){sc.getMemoryValue<int32_t>(memoryAddr);},
		[](Scanner& sc, uintptr_t memoryAddr){ImGui::Text("%d",  sc.getMemoryValue<int32_t>(memoryAddr));}
		
	},
	{
		"int64",
		[](Scanner& sc, const char* input){sc.scanExact<int64_t>(std::stoll(input));},
		[](Scanner& sc, const char* input){sc.rescanExact<int64_t>(std::stoll(input));},
		[](Scanner& sc, int index, const char* input){sc.write<int64_t>(index, std::stoll(input));},
		[](Scanner& sc, uintptr_t memoryAddrList){sc.getMemoryValue<int64_t>(memoryAddrList);},
		[](Scanner& sc, uintptr_t memoryAddr){ImGui::Text("%lld",  sc.getMemoryValue<int64_t>(memoryAddr));}
	},
	{
		"float",
		[](Scanner& sc, const char* input){sc.scanExact<float>(std::stof(input));},
		[](Scanner& sc, const char* input){sc.rescanExact<float>(std::stof(input));},
		[](Scanner& sc, int index, const char* input){sc.write<float>(index, std::stof(input));},
		[](Scanner& sc, uintptr_t memoryAddrList){sc.getMemoryValue<float>(memoryAddrList);},
		[](Scanner& sc, uintptr_t memoryAddr){ImGui::Text("%f",  sc.getMemoryValue<float>(memoryAddr));}
	}
};
