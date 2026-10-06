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
	void(*writeMemory)(Scanner&, uintptr_t memoryAddr, const char* input);
	void(*getMemoryValue)(Scanner&, uintptr_t memoryAddr);
	void(*renderMemoryValue)(Scanner&, uintptr_t memoryAddr);
	void(*scanUnknown)(Scanner&);
	void(*rescanLower)(Scanner&);
	void(*rescanGreater)(Scanner&);

    std::string (*valueToString)(Scanner&, uintptr_t memoryAddr);
};

inline const TypeInfo types[] =
{
	{
		"int32",
		[](Scanner& sc, const char* input){sc.scanExact<int32_t>(std::stoi(input));},
		[](Scanner& sc, const char* input){sc.rescanExact<int32_t>(std::stoi(input));},
		[](Scanner& sc, uintptr_t memoryAddr, const char* input){sc.write<int32_t>(memoryAddr, std::stoi(input));},
		[](Scanner& sc, uintptr_t memoryAddr){sc.getMemoryValue<int32_t>(memoryAddr);},
		[](Scanner& sc, uintptr_t memoryAddr){ImGui::Text("%d",  sc.getMemoryValue<int32_t>(memoryAddr));},
		[](Scanner& sc){sc.scanUnknown<int32_t>();},
		[](Scanner& sc){sc.rescanLower<int32_t>();},
		[](Scanner& sc){sc.rescanGreater<int32_t>();},
        [](Scanner& sc, uintptr_t memoryAddr){return std::format("{}", sc.getMemoryValue<int32_t>(memoryAddr));},
		
	},
	{
		"int64",
		[](Scanner& sc, const char* input){sc.scanExact<int64_t>(std::stoll(input));},
		[](Scanner& sc, const char* input){sc.rescanExact<int64_t>(std::stoll(input));},
		[](Scanner& sc, uintptr_t memoryAddr, const char* input){sc.write<int64_t>(memoryAddr, std::stoi(input));},
		[](Scanner& sc, uintptr_t memoryAddrList){sc.getMemoryValue<int64_t>(memoryAddrList);},
		[](Scanner& sc, uintptr_t memoryAddr){ImGui::Text("%lld",  sc.getMemoryValue<int64_t>(memoryAddr));},
		[](Scanner& sc){sc.scanUnknown<int64_t>();},
		[](Scanner& sc){sc.rescanLower<int64_t>();},
		[](Scanner& sc){sc.rescanGreater<int64_t>();},
        [](Scanner& sc, uintptr_t memoryAddr){return std::format("{}", sc.getMemoryValue<int64_t>(memoryAddr));},
	},
	{
		"float",
		[](Scanner& sc, const char* input){sc.scanExact<float>(std::stof(input));},
		[](Scanner& sc, const char* input){sc.rescanExact<float>(std::stof(input));},
		[](Scanner& sc, uintptr_t memoryAddr, const char* input){sc.write<float>(memoryAddr, std::stoi(input));},
		[](Scanner& sc, uintptr_t memoryAddrList){sc.getMemoryValue<float>(memoryAddrList);},
		[](Scanner& sc, uintptr_t memoryAddr){ImGui::Text("%f",  sc.getMemoryValue<float>(memoryAddr));},
		[](Scanner& sc){sc.scanUnknown<float>();},
		[](Scanner& sc){sc.rescanLower<float>();},
		[](Scanner& sc){sc.rescanGreater<float>();},
        [](Scanner& sc, uintptr_t memoryAddr){return std::format("{}", sc.getMemoryValue<float>(memoryAddr));},
	},
    {
    "uintptr_t",
    [](Scanner& sc, const char* input){sc.scanExact<uintptr_t>(std::stoull(input, nullptr, 16));},
    [](Scanner& sc, const char* input){sc.rescanExact<uintptr_t>(std::stoull(input, nullptr, 16));},
    [](Scanner& sc, uintptr_t memoryAddr, const char* input){sc.write<uintptr_t>(memoryAddr, std::stoi(input));},
    [](Scanner& sc, uintptr_t memoryAddr){sc.getMemoryValue<uintptr_t>(memoryAddr);},
    [](Scanner& sc, uintptr_t memoryAddr){ImGui::Text("0x%lX",  sc.getMemoryValue<uintptr_t>(memoryAddr));},
    [](Scanner& sc){sc.scanUnknown<uintptr_t>();},
    [](Scanner& sc){sc.rescanLower<uintptr_t>();},
    [](Scanner& sc){sc.rescanGreater<uintptr_t>();},
    [](Scanner& sc, uintptr_t memoryAddr){return std::format("0x{:X}", sc.getMemoryValue<uintptr_t>(memoryAddr));},
    },
};
