#include <MultiLevelPointer.h>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <sys/types.h>
#include <vector>
#include "Scanner.h"
#include <chrono>  
#include <print>



MultiLevelPointer::MultiLevelPointer(Scanner& sc): sc(sc)
{
}

PointerNode MultiLevelPointer::getMultilevelPointer(uintptr_t memAddr, uintptr_t offset, uintptr_t maxDepth, uintptr_t currentDepth)
{
    std::string indent(currentDepth * 2, ' ');           // indents by depth, so the output looks like a tree

    if (offset == 0)
        std::println("{}[d{}] searching 0x{:X}", indent, currentDepth, memAddr);

    PointerNode pointerNode;

    if (offset == 0 && sc.isStatic(memAddr))
    {
        pointerNode.found = true;
        pointerNode.path = sc.findPath(memAddr);
        pointerNode.offset.push_back(memAddr - sc.getBaseFromPath(pointerNode.path));
        return pointerNode;
    }
    if (offset > maxOffset || currentDepth >= maxDepth) return pointerNode;
    std::vector<uintptr_t> memoryAddrList = findPointersTo(memAddr - offset);
    for (int i = 0; i < memoryAddrList.size(); i++)
    {
        pointerNode = getMultilevelPointer(memoryAddrList[i], 0, maxDepth,currentDepth+1);
        if (pointerNode.found == true)
        {
            pointerNode.offset.push_back(offset);
            return pointerNode;
        }
    }
    if (pointerNode.found ==true)
    {
        return pointerNode;
    }
    pointerNode = getMultilevelPointer(memAddr, offset + 8, maxDepth, currentDepth);
    return pointerNode;
}

std::optional<uintptr_t> MultiLevelPointer::getAdress(std::string path,std::vector<uintptr_t>offset)
{
    if (offset.empty()) return std::nullopt;
    sc.refreshMaps();
    uintptr_t base = sc.getBaseFromPath(path);
    uintptr_t memoryAddr = base + offset[0];
    std::println("base 0x{:X} + 0x{:X} = 0x{:X}", base, offset[0], memoryAddr);

    for (size_t i = 1; i < offset.size(); i++)
    {
        uintptr_t pointer = sc.getMemoryValue<uintptr_t>(memoryAddr);
        std::println("  read 0x{:X} -> 0x{:X}   + 0x{:X} = 0x{:X}",
                     memoryAddr, pointer, offset[i], pointer + offset[i]);
        if (pointer == 0) return std::nullopt;
        memoryAddr = pointer + offset[i];
    }
    return memoryAddr;
}

std::vector<TableContent> MultiLevelPointer::buildPointerMap()
{
    std::vector<TableContent> table;
    const std::vector<MemorySnapshot>& snapshotBefore = sc.getMemorySnapshot();
    uintptr_t tempValue;
    
    for (int i = 0; snapshotBefore.size() > i ; i++)
    {
        for (size_t j = 0; j + sizeof(uintptr_t) <= snapshotBefore[i].bytes.size(); j += sizeof(uintptr_t))
        {
            std::memcpy(&tempValue, snapshotBefore[i].bytes.data() + j,sizeof(uintptr_t));
            if (sc.isValidAddress(tempValue))
            {
                TableContent tableContent;
                tableContent.memAddr = snapshotBefore[i].start + j;
                tableContent.value = tempValue;
                table.push_back(tableContent);
            }
        }
    
    }
    std::sort(table.begin(), table.end());
    std::println("pointer map: {} entries (~{} MB)", table.size(), table.size() * 16 / (1024 * 1024));
    return table;
}

std::vector<uintptr_t> MultiLevelPointer::findPointersTo(uintptr_t memAddr)
{
    std::vector<uintptr_t> result;

    TableContent key;
    key.memAddr = 0;
    key.value = memAddr;

    size_t i = std::lower_bound(table.begin(), table.end(), key) - table.begin();

    while (i < table.size() && table[i].value == memAddr)
    {
        result.push_back(table[i].memAddr);   
        i++;
    }
    return result;

}

PointerNode MultiLevelPointer::test(uintptr_t target, uintptr_t maxDepth)
{
    auto t0 = std::chrono::steady_clock::now();
    sc.newScan();                                              // 1. snapshot
    auto t1 = std::chrono::steady_clock::now();
    table = buildPointerMap();                                 // 2. build table
    auto t2 = std::chrono::steady_clock::now();
    PointerNode result = getMultilevelPointer(target, 0, maxDepth, 0);   // 3. search
    auto t3 = std::chrono::steady_clock::now();

    auto secs = [](auto a, auto b) { return std::chrono::duration<double>(b - a).count(); };
    std::println("snapshot {:.2f}s | build table {:.2f}s | search {:.2f}s",
                 secs(t0, t1), secs(t1, t2), secs(t2, t3));

    return result;
}

