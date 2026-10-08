#pragma once
#include <cstdint>
#include <optional>
#include <sys/types.h>
#include <vector>
#include "Scanner.h"

struct TableContent
{
    uintptr_t memAddr;
    uintptr_t value;
    bool operator<(const TableContent& other) const { return value < other.value; }
};

struct PointerNode
{
    std::string path;
    std::vector<uintptr_t> offset;
    bool found{false};
};
class MultiLevelPointer
{
    private:
        uintptr_t maxOffset = 0xFFF;
        Scanner& sc;
        std::vector<TableContent> table{};
    public:
        explicit MultiLevelPointer(Scanner& sc);
        std::vector<PointerNode> getMultilevelPointer(uintptr_t memAddr, uintptr_t offset, uintptr_t maxDepth, uintptr_t currentDepth);
        std::optional<uintptr_t> getAdress(std::string path,std::vector<uintptr_t>offset);
        std::vector<TableContent> buildPointerMap();
        std::vector<uintptr_t> findPointersTo(uintptr_t memAddr);
        std::vector<PointerNode> test(uintptr_t target, uintptr_t maxDepth);

};
