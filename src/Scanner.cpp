#include "Scanner.h"
#include <cstdint>
#include <cstring>
#include <sys/types.h>
#include <vector>



Scanner::Scanner()
{
}


bool Scanner::newScan()
{
	Scanner::memorySnapshot.clear();
	proc.attatch();
	proc.parceMaps();
	for (int i = 0; i < proc.regions.size(); i++)
	{
		MemorySnapshot snapshot;
		snapshot.bytes = proc.readMemory(proc.regions[i].start, proc.regions[i].end - proc.regions[i].start);
		snapshot.start = proc.regions[i].start;
		Scanner::memorySnapshot.push_back(snapshot);
	}
	proc.detatch();
	return true;
}

std::vector<uintptr_t> Scanner::getMemoryAddrList()
{
	return Scanner::memoryAddrList;
}

std::vector<std::string> Scanner::getProcessNames()
{
    return proc.getProcessNames();
}

void Scanner::processConnect(std::string procName)
{
    proc.getId(procName);
}

void Scanner::updateProcessNames()
{
    proc.getAllNames();
}

bool Scanner::isStatic(uintptr_t memoryAddr)
{
    return proc.isStatic(memoryAddr);
}

std::vector<uintptr_t> Scanner::findPointersTo(uintptr_t memoryAddr)
{
    std::vector<uintptr_t> memoryList;
    uintptr_t tempValue = memoryAddr;
    std::vector<MemorySnapshot>& snapshotBefore = Scanner::memorySnapshot;
    
    for (int i = 0; snapshotBefore.size() > i ; i++)
    {
        for (size_t j = 0; j + sizeof(uintptr_t) <= snapshotBefore[i].bytes.size(); j += scanStep<uintptr_t>())
        {
            std::memcpy(&memoryAddr, snapshotBefore[i].bytes.data() + j,sizeof(uintptr_t));
            if (tempValue == memoryAddr) 
            {
                memoryList.push_back(snapshotBefore[i].start + j);
            }
        }
    
    }
    return memoryList;
}

std::string Scanner::findPath(uintptr_t memoryAddr)
{
    return proc.getPath(memoryAddr);
}

uintptr_t Scanner::getBaseFromPath(std::string path)
{
    return proc.getBaseFromPath(path);
}
std::vector<MemorySnapshot> Scanner::getMemorySnapshot()
{
    return memorySnapshot;
}

bool Scanner::isValidAddress(uintptr_t memoryAddr)
{
    return proc.isValidAddress(memoryAddr);
}
